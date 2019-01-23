#ifndef ARCHIVEPARSE_BSA_FILESYSTEM_LAYER_H
#define ARCHIVEPARSE_BSA_FILESYSTEM_LAYER_H

#include <archiveparse/FilesystemLayer.h>
#include <archiveparse/WindowsHandle.h>
#include <archiveparse/BSATypes.h>

#include <string>
#include <variant>
#include <vector>
#include <unordered_map>

namespace archiveparse {
	class BSAFilesystemLayer final : public FilesystemLayer {
	public:
		BSAFilesystemLayer(std::string &&path, std::unordered_map<std::string, std::string> &&options);
		~BSAFilesystemLayer();

		virtual std::unique_ptr<File> lookup(const std::string &filename) const override;
		virtual std::vector<std::string> enumerate() const override;

	private:
		enum class Type {
			Morrowind,
			Current
		};

		struct MorrowindData {
			unsigned int fileNameCodePage;
			MorrowindBSAHeader header;
			std::vector<MorrowindFileSizeAndOffset> fileSizesAndOffsets;
			std::vector<uint32_t> nameOffsets;
			std::vector<char> fileNames;
			std::vector<MorrowindBSAFileNameHash> fileNameHashes;
			size_t headerSize;
		};

		struct FolderData {
			CurrentBSAFileNameHash nameHash;
			std::string name;
			std::vector<CurrentBSAFile> files;
			std::vector<std::string> fileNames;
		};

		struct CurrentData {
			unsigned int fileNameCodePage;
			CurrentBSAHeader header;
			std::vector<FolderData> folders;
		};

		friend bool operator <(const BSAFilesystemLayer::FolderData &a, const CurrentBSAFileNameHash &b);
		friend bool operator <(const CurrentBSAFileNameHash &a, const BSAFilesystemLayer::FolderData &b);
		friend bool operator <(const BSAFilesystemLayer::FolderData &a, const BSAFilesystemLayer::FolderData &b);
		

		std::unique_ptr<File> lookupInData(const MorrowindData &data, const std::string &filename) const;
		std::vector<std::string> enumerateData(const MorrowindData &data) const;

		std::unique_ptr<File> lookupInData(const CurrentData &data, const std::string &filename) const;
		std::vector<std::string> enumerateData(const CurrentData &data) const;

		static std::string sanitizeFilename(const std::string &filename, unsigned int codepage);

		WindowsHandle m_handle;
		std::variant<MorrowindData, CurrentData> m_data;
	};
}

#endif
