#include <archiveparse/BSAFilesystemLayer.h>
#include <archiveparse/BSATypes.h>
#include <archiveparse/BSAFile.h>
#include <archiveparse/EncodingUtilities.h>
#include <archiveparse/BSAHash.h>

#include <Windows.h>
#include <comdef.h>

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

	BSAFilesystemLayer::BSAFilesystemLayer(std::string &&path, std::unordered_map<std::string, std::string> &&options) {
		HANDLE rawHandle = CreateFile(utf8ToWide(path).c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL, NULL);
		if (rawHandle == INVALID_HANDLE_VALUE)
			_com_raise_error(HRESULT_FROM_WIN32(GetLastError()));


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
			_com_raise_error(HRESULT_FROM_WIN32(GetLastError()));

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

			header.read(reinterpret_cast<char *>(data.fileNameHashes.data()), data.fileNameHashes.size() * sizeof(MorrowindFileNameHash));

			data.headerSize = static_cast<size_t>(header.tellg());
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

	std::unique_ptr<File> BSAFilesystemLayer::lookupInData(const MorrowindData &data, const std::string &filename) const {
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

		auto morrowindFilename = wideToCodepage(lowercaseFilename, data.fileNameCodePage);
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

	std::unique_ptr<File> BSAFilesystemLayer::lookupInData(const CurrentData &data, const std::string &filename) const {
		throw std::logic_error("unimplemented");
	}

	std::vector<std::string> BSAFilesystemLayer::enumerateData(const CurrentData &data) const {
		throw std::logic_error("unimplemented");
	}
}
