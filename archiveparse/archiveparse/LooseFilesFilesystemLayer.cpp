#include <archiveparse/LooseFilesFilesystemLayer.h>
#include <archiveparse/EncodingUtilities.h>
#include <archiveparse/WindowsHandle.h>
#include <archiveparse/WindowsError.h>
#include <archiveparse/LooseFile.h>

#include <unordered_map>
#include <stdexcept>
#include <sstream>

#include <Windows.h>

namespace archiveparse {
	LooseFilesFilesystemLayer::LooseFilesFilesystemLayer(std::string &&path, std::unordered_map<std::string, std::string> &&options) : m_path(std::move(path)) {
		auto it = options.find("FilenameEncoding");
		if (it == options.end()) {
			m_fileNameCodePage = 1252;
		}
		else {
			m_fileNameCodePage = static_cast<unsigned int>(std::stoul(it->second));
		}
	}

	LooseFilesFilesystemLayer::~LooseFilesFilesystemLayer() = default;

	std::unique_ptr<File> LooseFilesFilesystemLayer::lookup(const std::string &filename) const {
		std::wstringstream namestream;
		namestream << utf8ToWide(m_path) << "\\" << codepageToWide(filename, m_fileNameCodePage);

		HANDLE handle = CreateFile(namestream.str().c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL, NULL);

		if (handle == INVALID_HANDLE_VALUE) {
			auto error = GetLastError();

			if (error == ERROR_FILE_NOT_FOUND || error == ERROR_PATH_NOT_FOUND)
				return {};
			else
				throw WindowsError(error);
		}

		WindowsHandle handleHolder(handle);

		return std::make_unique<LooseFile>(std::move(handleHolder));
	}

	std::vector<std::string> LooseFilesFilesystemLayer::enumerate() const {
		throw std::logic_error("unimplented");
	}
}

