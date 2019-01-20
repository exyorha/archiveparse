#ifndef ARCHIVEPARSE_FILESYSTEM_LAYER_FACTORY_H
#define ARCHIVEPARSE_FILESYSTEM_LAYER_FACTORY_H

#include <memory>
#include <string>
#include <unordered_map>

namespace archiveparse {
	class FilesystemLayer;

	class FilesystemLayerFactory {
	public:
		FilesystemLayerFactory();
		~FilesystemLayerFactory();

		FilesystemLayerFactory(const FilesystemLayerFactory &other) = delete;
		FilesystemLayerFactory &operator =(const FilesystemLayerFactory &other) = delete;

		std::unique_ptr<FilesystemLayer> createLayer(const char *providerName, std::string &&path, std::unordered_map<std::string, std::string> &&options);

	private:
		struct LayerFactory {
			const char *name;
			std::unique_ptr<FilesystemLayer>(FilesystemLayerFactory::*factoryFunc)(std::string &&path, std::unordered_map<std::string, std::string> &&options);
		};

		template<typename T>
		std::unique_ptr<FilesystemLayer> layerFactory(std::string &&path, std::unordered_map<std::string, std::string> &&options);

		static const LayerFactory m_factories[];
	};
}

#endif
