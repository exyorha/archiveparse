#include <archiveparse/BSAFilesystemLayer.h>
#include <archiveparse/BSATypes.h>
#include <archiveparse/BSAFile.h>
#include <archiveparse/EncodingUtilities.h>
#include <archiveparse/BSAHash.h>
#include <archiveparse/BSACompressedFile.h>
#include <archiveparse/WindowsError.h>

#include <fstream>
#include <sstream>
#include <algorithm>

#include <io.h>
#include <fcntl.h>

namespace archiveparse {
	struct FileDeleter {
		void operator()(FILE *handle) const {
			fclose(handle);
		}
	};

	template<typename T>
	void BSAFilesystemLayer::readFolders(BSAFilesystemLayer::CurrentData &data, std::istream &header) {
		std::vector<T> folders;

		header.seekg(data.header.folderOffset);
		folders.resize(data.header.folderCount);
		header.read(reinterpret_cast<char *>(folders.data()), folders.size() * sizeof(CurrentBSAFolder));

		data.folders.reserve(data.header.folderCount);

		for (const auto &folder : folders) {
			if (folder.offset == 0)
				continue;

			auto &outFolder = data.folders.emplace_back();

			outFolder.nameHash = folder.nameHash;

			header.seekg(folder.offset - data.header.totalFileNameLength);

			if (data.header.archiveFlags & BSAFolderNamesRetained) {
				uint8_t nameLength;
				header.read(reinterpret_cast<char *>(&nameLength), sizeof(nameLength));
				outFolder.name.resize(nameLength);
				header.read(outFolder.name.data(), outFolder.name.size());
				outFolder.name.erase(std::find(outFolder.name.begin(), outFolder.name.end(), '\0'), outFolder.name.end());
			}

			outFolder.files.resize(folder.count);
			header.read(reinterpret_cast<char *>(outFolder.files.data()), outFolder.files.size() * sizeof(CurrentBSAFile));
		}

	}

	BSAFilesystemLayer::BSAFilesystemLayer(std::string &&path, std::unordered_map<std::string, std::string> &&options) {
		HANDLE rawHandle = CreateFile(utf8ToWide(path).c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL, NULL);
		if (rawHandle == INVALID_HANDLE_VALUE)
			throw WindowsError();


		m_handle.reset(rawHandle);

		HANDLE rawDup;

		if (!DuplicateHandle(
			GetCurrentProcess(),
			m_handle.get(),
			GetCurrentProcess(),
			&rawDup,
			GENERIC_READ,
			FALSE,
			0))
			throw WindowsError();

		WindowsHandle dupHandle(rawDup);

		int osHandle = _open_osfhandle(reinterpret_cast<intptr_t>(dupHandle.get()), _O_RDONLY);
		if (osHandle < 0) {
			throw std::runtime_error("_open_osfhandle failed");
		}
		std::unique_ptr<FILE, FileDeleter> fhandle(_fdopen(osHandle, "rb"));
		if (!fhandle)
			throw std::runtime_error("fdopen failed");

		dupHandle.release();

		std::ifstream header(fhandle.get());
		header.exceptions(std::ios::failbit | std::ios::eofbit | std::ios::badbit);
		
		uint32_t magic;
		header.read(reinterpret_cast<char *>(&magic), sizeof(magic));

		if (magic == MorrowindBSAVersion) {

			m_data = MorrowindData();
			auto &data = std::get<MorrowindData>(m_data);
			data.header.version = magic;

			auto it = options.find("FilenameEncoding");
			if (it == options.end()) {
				data.fileNameCodePage = 1252;
			}
			else {
				data.fileNameCodePage = static_cast<unsigned int>(std::stoul(it->second));
			}

			header.read(reinterpret_cast<char *>(&data.header) + sizeof(data.header.version),
				sizeof(data.header) - sizeof(data.header.version));

			data.fileSizesAndOffsets.resize(data.header.fileCount);
			data.nameOffsets.resize(data.header.fileCount);

			header.read(reinterpret_cast<char *>(data.fileSizesAndOffsets.data()),
				data.fileSizesAndOffsets.size() * sizeof(MorrowindFileSizeAndOffset));

			header.read(reinterpret_cast<char *>(data.nameOffsets.data()), data.nameOffsets.size() * sizeof(uint32_t));

			auto namesSize = data.header.hashOffset - data.header.fileCount * (sizeof(MorrowindFileSizeAndOffset) + sizeof(uint32_t));

			data.fileNames.resize(namesSize);

			header.read(data.fileNames.data(), data.fileNames.size());

			data.fileNameHashes.resize(data.header.fileCount);

			header.read(reinterpret_cast<char *>(data.fileNameHashes.data()), data.fileNameHashes.size() * sizeof(MorrowindBSAFileNameHash));

			data.headerSize = static_cast<size_t>(header.tellg());
		}
		else if (magic == CurrentBSAMagic) {
			m_data = CurrentData();
			auto &data = std::get<CurrentData>(m_data);
			data.header.magic = magic;

			auto it = options.find("FilenameEncoding");
			if (it == options.end()) {
				data.fileNameCodePage = CP_UTF8;
			}
			else {
				data.fileNameCodePage = static_cast<unsigned int>(std::stoul(it->second));
			}

			header.read(reinterpret_cast<char *>(&data.header) + sizeof(data.header.magic),
				sizeof(data.header) - sizeof(data.header.magic));

			if (data.header.version < OblivionBSAVersion || data.header.version > SkyrimSEBSAVersion) {
				std::stringstream stream;
				stream << "Unsupported BSA file: version " << data.header.version;
				throw std::runtime_error(stream.str());
			}

			if (data.header.version < SkyrimSEBSAVersion) {
				readFolders<CurrentBSAFolder>(data, header);
			}
			else {
				readFolders<SSEBSAFolder>(data, header);
			}

			size_t expectedPosition = data.header.folderOffset;

			if (data.header.version < SkyrimSEBSAVersion) {
				expectedPosition += data.header.folderCount * sizeof(CurrentBSAFolder);
			}
			else {
				expectedPosition += data.header.folderCount * sizeof(SSEBSAFolder);
				
			}

			if (data.header.archiveFlags & BSAFolderNamesRetained) {
				expectedPosition += sizeof(unsigned char) * data.header.folderCount;
				expectedPosition += data.header.totalFolderNameLength;
			}

			expectedPosition += data.header.fileCount * sizeof(CurrentBSAFile);

			if (data.header.version < SkyrimSEBSAVersion) {
				if (header.tellg() != expectedPosition) {
					throw std::logic_error("current position is unexpected");
				}
			}
			else {
				header.seekg(expectedPosition);
			}
			
			if (data.header.archiveFlags & BSAFileNamesRetained) {
				std::vector<char> nameHeap;
				nameHeap.resize(data.header.totalFileNameLength);
				header.read(nameHeap.data(), nameHeap.size());

				auto beginPos = nameHeap.begin();

				for (auto &outFolder : data.folders) {
					outFolder.fileNames.resize(outFolder.files.size());

					for (auto &name : outFolder.fileNames) {
						auto end = std::find(beginPos, nameHeap.end(), '\0');
						if (end == nameHeap.end())
							throw std::logic_error("null terminator not found");

						name.assign(beginPos, end);

						beginPos = end + 1;
					}
				}
			}
		}
		else {
			std::stringstream stream;
			stream << "Unsupported BSA file: magic " << std::hex << magic;
			throw std::runtime_error(stream.str());
		}
	}

	BSAFilesystemLayer::~BSAFilesystemLayer() {

	}

	std::unique_ptr<File> BSAFilesystemLayer::lookup(const std::string &filename) const {
		return std::visit([&](const auto &val) {
			return lookupInData(val, filename);
		}, m_data);
	}

	std::vector<std::string> BSAFilesystemLayer::enumerate() const {
		return std::visit([&](const auto &val) {
			return enumerateData(val);
		}, m_data);
	}

	std::string BSAFilesystemLayer::sanitizeFilename(const std::string &filename, unsigned int codepage) {
		auto wideFilename = utf8ToWide(filename);
		std::wstring lowercaseFilename;

		auto result = LCMapStringEx(LOCALE_NAME_INVARIANT, LCMAP_LOWERCASE, wideFilename.data(), wideFilename.size(), lowercaseFilename.data(), lowercaseFilename.size(), nullptr, nullptr, 0);
		if (result == 0)
			throw std::runtime_error("LCMapStringEx failed");

		lowercaseFilename.resize(result);

		result = LCMapStringEx(LOCALE_NAME_INVARIANT, LCMAP_LOWERCASE, wideFilename.data(), wideFilename.size(), lowercaseFilename.data(), lowercaseFilename.size(), nullptr, nullptr, 0);
		if (result == 0)
			throw std::runtime_error("LCMapStringEx failed");

		for (auto &ch : lowercaseFilename) {
			if (ch == L'/')
				ch = L'\\';
		}

		return wideToCodepage(lowercaseFilename, codepage);
	}

	std::unique_ptr<File> BSAFilesystemLayer::lookupInData(const MorrowindData &data, const std::string &filename) const {
		auto morrowindFilename = sanitizeFilename(filename, data.fileNameCodePage);
		auto hash = calculateMorrowindHash(morrowindFilename);

		auto range = std::equal_range(data.fileNameHashes.begin(), data.fileNameHashes.end(), hash);

		for (auto item = range.first; item != range.second; item++) {
			auto index = item - data.fileNameHashes.begin();
			auto nameOffset = data.nameOffsets[index];

			auto nameBegin = data.fileNames.begin() + nameOffset;
			auto nameEnd = std::find(nameBegin, data.fileNames.end(), 0);
			if (std::equal(morrowindFilename.begin(), morrowindFilename.end(), nameBegin, nameEnd)) {
				return std::make_unique<BSAFile>(m_handle.get(), data.fileSizesAndOffsets[index].fileOffset + data.headerSize, data.fileSizesAndOffsets[index].fileSize);
			}
		}

		return {};
	}

	std::vector<std::string> BSAFilesystemLayer::enumerateData(const MorrowindData &data) const {
		std::vector<std::string> result;
		result.reserve(data.header.fileCount);

		for (auto nameOffset : data.nameOffsets) {
			auto nameBegin = data.fileNames.begin() + nameOffset;
			auto endOfString = std::find(nameBegin, data.fileNames.end(), 0);
			result.emplace_back(codepageToUtf8(std::string(nameBegin, endOfString), data.fileNameCodePage));
		}

		return result;
	}
	
	inline bool operator <(const BSAFilesystemLayer::FolderData &a, const CurrentBSAFileNameHash &b) {
		return a.nameHash < b;
	}
	
	inline bool operator <(const CurrentBSAFileNameHash &a, const BSAFilesystemLayer::FolderData &b) {
		return a < b.nameHash;
	}
	
	inline bool operator <(const BSAFilesystemLayer::FolderData &a, const BSAFilesystemLayer::FolderData &b) {
		return a.nameHash < b.nameHash;
	}

	inline bool operator <(const CurrentBSAFile &a, const CurrentBSAFileNameHash &b) {
		return a.nameHash < b;
	}

	inline bool operator <(const CurrentBSAFileNameHash &a, const CurrentBSAFile &b) {
		return a < b.nameHash;
	}

	inline bool operator <(const CurrentBSAFile &a, const CurrentBSAFile &b) {
		return a.nameHash < b.nameHash;
	}

	std::unique_ptr<File> BSAFilesystemLayer::lookupInData(const CurrentData &data, const std::string &filename) const {
		auto bsaFilename = sanitizeFilename(filename, data.fileNameCodePage);

		auto directoryDelimiter = std::find(bsaFilename.rbegin(), bsaFilename.rend(), '\\');
		if (directoryDelimiter == bsaFilename.rend())
			throw std::logic_error("directory delimiter not found (files in root directory are not supported yet)");

		CurrentBSAFileNameHash directoryHash(calculateCurrentHash(bsaFilename.begin(), directoryDelimiter.base() - 1, 0));
		
		auto directoryRange = std::equal_range(data.folders.begin(), data.folders.end(), directoryHash);

		if (directoryRange.second - directoryRange.first > 1 && !(data.header.archiveFlags & BSAFolderNamesRetained)) {
			throw std::logic_error("hash collision in folder names and names are not retained");
		}

		for (auto item = directoryRange.first; item != directoryRange.second; item++) {
			const auto &directory = *item;

			if (!(data.header.archiveFlags & BSAFolderNamesRetained) || std::equal(directory.name.begin(), directory.name.end(), bsaFilename.begin(), directoryDelimiter.base() - 1)) {
				
				std::string baseName(directoryDelimiter.base(), bsaFilename.end());
				CurrentBSAFileNameHash fileHash(calculateCurrentHashWithExtension(baseName));

				auto fileRange = std::equal_range(directory.files.begin(), directory.files.end(), fileHash);

				if (fileRange.second - fileRange.first > 1 && !(data.header.archiveFlags & BSAFileNamesRetained)) {
					throw std::logic_error("hash collision in file names and names are not retained");
				}

				for (auto fileItem = fileRange.first; fileItem != fileRange.second; fileItem++) {
					if (data.header.archiveFlags & BSAFileNamesRetained) {
						const auto &name = directory.fileNames[fileItem - directory.files.begin()];
						if (name != baseName)
							continue;					
					}

					const auto &file = *fileItem;

					auto size = file.size & BSAFileSizeMask;
					auto offset = file.offset;

					if (data.header.version >= SkyrimBSAVersion && (data.header.archiveFlags & BSAHasEmbeddedFileNames_Skyrim)) {
						unsigned char nameLength;
						DWORD bytesRead;
						OVERLAPPED overlapped;
						ZeroMemory(&overlapped, sizeof(overlapped));
						overlapped.Offset = static_cast<DWORD>(static_cast<uint64_t>(offset));
						overlapped.OffsetHigh = static_cast<DWORD>(static_cast<uint64_t>(offset) >> 32);

						if (!ReadFile(m_handle.get(), &nameLength, sizeof(nameLength), &bytesRead, &overlapped) || bytesRead != sizeof(nameLength))
							throw std::runtime_error("ReadFile failed");

						offset += nameLength + 1;
						size -= nameLength + 1;
					}

					if (((data.header.archiveFlags & BSACompressedByDefault) != 0) != ((file.size & BSAFileSizeFlagCompressed) != 0)) {
						auto algorithm = BSACompressedFile::Algorithm::Zlib;

						if (data.header.version >= SkyrimSEBSAVersion)
							algorithm = BSACompressedFile::Algorithm::LZ4;

						return std::make_unique<BSACompressedFile>(m_handle.get(), offset, size, algorithm);
					}
					else {
						return std::make_unique<BSAFile>(m_handle.get(), offset, size);
					}
				}

				break;
			}
		}

		return {};
	}

	std::vector<std::string> BSAFilesystemLayer::enumerateData(const CurrentData &data) const {
		std::vector<std::string> names;

		for (const auto &folder : data.folders) {
			std::string folderName;
			if (data.header.archiveFlags & BSAFolderNamesRetained) {
				folderName = folder.name;
			}
			else {
				folderName = folder.nameHash.toString();
			}

			size_t index = 0;
			for (const auto &file : folder.files) {
				std::stringstream fullName;

				if (!folderName.empty())
					fullName << folderName << "\\";
				
				if (data.header.archiveFlags & BSAFileNamesRetained) {
					fullName << folder.fileNames[index];
				}
				else {
					fullName << file.nameHash.toString();
				}

				index++;

				names.emplace_back(std::move(fullName.str()));
			}
		}

		return names;
	}
}
