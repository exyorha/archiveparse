#ifndef ARCHIVEPARSE_LOOSE_FILES_FILESYSTEM_LAYER_H
#define ARCHIVEPARSE_LOOSE_FILES_FILESYSTEM_LAYER_H

#include <archiveparse/FilesystemLayer.h>

#include <string>
#include <unordered_map>

namespace archiveparse {
	class LooseFilesFilesystemLayer final : public FilesystemLayer {
	public:
		LooseFilesFilesystemLayer(std::string &&path, std::unordered_map<std::string, std::string> &&options);
		virtual ~LooseFilesFilesystemLayer();

		virtual std::unique_ptr<File> lookup(const std::string &filename) const override;
		virtual std::vector<std::string> enumerate() const override;
	};
}

#endif
