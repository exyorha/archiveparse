#ifndef ARCHIVEPARSE_FILESYSTEM_LAYER_H
#define ARCHIVEPARSE_FILESYSTEM_LAYER_H

#include <memory>
#include <string>
#include <vector>

namespace archiveparse {
	class File;

	class FilesystemLayer {
	protected:
		FilesystemLayer();

	public:
		virtual ~FilesystemLayer();

		FilesystemLayer(const FilesystemLayer &other) = delete;
		FilesystemLayer &operator =(const FilesystemLayer &other) = delete;

		virtual std::unique_ptr<File> lookup(const std::string &filename) const = 0;
		virtual std::vector<std::string> enumerate() const = 0;
	};
}

#endif
