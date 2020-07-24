#include <archiveparse/FilesystemLayerFactory.h>
#include <archiveparse/FilesystemLayer.h>
#include <archiveparse/BSAFilesystemLayer.h>
#include <archiveparse/LooseFilesFilesystemLayer.h>

#include <sstream>

namespace archiveparse {
	const FilesystemLayerFactory::LayerFactory FilesystemLayerFactory::m_factories[]{
		{ "BSA", &FilesystemLayerFactory::layerFactory<BSAFilesystemLayer> },
		{ "LooseFiles", &FilesystemLayerFactory::layerFactory<LooseFilesFilesystemLayer> },
	};

	FilesystemLayerFactory::FilesystemLayerFactory() = default;

	FilesystemLayerFactory::~FilesystemLayerFactory() = default;

	std::unique_ptr<FilesystemLayer> FilesystemLayerFactory::createLayer(const char *providerName, std::string &&path, std::unordered_map<std::string, std::string> &&options) {
		auto factory = static_cast<LayerFactory *>(
			bsearch(
				providerName,
				m_factories,
				sizeof(m_factories) / sizeof(m_factories[0]),
				sizeof(m_factories[0]),
				[](const void *a, const void *b) -> int {
					return strcmp(static_cast<const char *>(a), static_cast<const LayerFactory *>(b)->name);
				}
			)
		);

		if (factory == nullptr) {
			std::stringstream error;
			error << "Unsupported filesystem layer type: " << providerName;
			throw std::runtime_error(error.str());
		}

		return (this->*factory->factoryFunc)(std::move(path), std::move(options));
	}

	template<typename T>
	std::unique_ptr<FilesystemLayer> FilesystemLayerFactory::layerFactory(std::string &&path, std::unordered_map<std::string, std::string> &&options) {
		return std::make_unique<T>(std::move(path), std::move(options));
	}
}
