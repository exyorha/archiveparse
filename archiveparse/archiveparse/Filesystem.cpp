#include <archiveparse/Filesystem.h>
#include <archiveparse/FilesystemLayer.h>
#include <archiveparse/File.h>

#include <algorithm>

namespace archiveparse {
	Filesystem::Filesystem() = default;

	Filesystem::~Filesystem() = default;

	void Filesystem::addLayer(std::unique_ptr<FilesystemLayer> &&layer) {
		m_layers.emplace_back(std::move(layer));
	}

	void Filesystem::removeLayer(FilesystemLayer *layer) {
		m_layers.erase(std::remove_if(m_layers.begin(), m_layers.end(), [=](const std::unique_ptr<FilesystemLayer> &a) { return a.get() == layer; }), m_layers.end());
	}

	std::unique_ptr<File> Filesystem::lookup(const std::string &filename) const {
		for (auto it = m_layers.crbegin(); it != m_layers.crend(); it++) {
			auto result = (*it)->lookup(filename);
			if (result)
				return result;
		}

		return {};
	}

	std::vector<std::string> Filesystem::enumerate() const {
		std::vector<std::string> result;

		for (const auto &layer : m_layers) {
			auto layerResult = layer->enumerate();
			result.insert(result.end(), std::make_move_iterator(layerResult.begin()), std::make_move_iterator(layerResult.end()));
		}

		std::sort(result.begin(), result.end());
		result.erase(std::unique(result.begin(), result.end()), result.end());

		return result;
	}
}
