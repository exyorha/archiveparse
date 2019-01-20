#ifndef ARCHIVEPARSE_FILESYSTEM_H
#define ARCHIVEPARSE_FILESYSTEM_H

#include <memory>
#include <vector>

namespace archiveparse {
	class FilesystemLayer;
	class File;

	class Filesystem {
	public:
		Filesystem();
		~Filesystem();

		Filesystem(const Filesystem &other) = delete;
		Filesystem &operator =(const Filesystem &other) = delete;

		void addLayer(std::unique_ptr<FilesystemLayer> &&layer);
		void removeLayer(FilesystemLayer *layer);

		std::unique_ptr<File> lookup(const std::string &filename) const;
		std::vector<std::string> enumerate() const;

	private:
		std::vector<std::unique_ptr<FilesystemLayer>> m_layers;
	};
}

#endif
