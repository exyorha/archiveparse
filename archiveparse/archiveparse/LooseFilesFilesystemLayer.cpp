#include <archiveparse/LooseFilesFilesystemLayer.h>

#include <unordered_map>

namespace archiveparse {
	LooseFilesFilesystemLayer::LooseFilesFilesystemLayer(std::string &&path, std::unordered_map<std::string, std::string> &&options) {

	}

	LooseFilesFilesystemLayer::~LooseFilesFilesystemLayer() {

	}

	std::unique_ptr<File> LooseFilesFilesystemLayer::lookup(const std::string &filename) const {
		throw std::logic_error("unimplented");
	}

	std::vector<std::string> LooseFilesFilesystemLayer::enumerate() const {
		throw std::logic_error("unimplented");
	}
}

