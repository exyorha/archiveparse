#include <archiveparse/Filesystem.h>
#include <archiveparse/FilesystemLayer.h>
#include <archiveparse/FilesystemLayerFactory.h>
#include <archiveparse/File.h>

#include <fstream>

int main(int argc, char **argv) {
	archiveparse::Filesystem fs;
	archiveparse::FilesystemLayerFactory factory;
	fs.addLayer(factory.createLayer("BSA", "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Morrowind\\Data Files\\Morrowind.bsa", {}));
	fs.addLayer(factory.createLayer("BSA", "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Morrowind\\Data Files\\Bloodmoon.bsa", {}));
	fs.addLayer(factory.createLayer("BSA", "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Morrowind\\Data Files\\Tribunal.bsa", {}));
	//fs.addLayer(factory.createLayer("LooseFiles", "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Morrowind\\Data Files", {}));

	std::ofstream logfile;
	logfile.exceptions(std::ios::failbit | std::ios::badbit | std::ios::eofbit);
	logfile.open("C:\\projects\\archiveparse\\log.txt", std::ios::out | std::ios::trunc);

	static const char bom[] = { static_cast<char>(0xEF), static_cast<char>(0xBB), static_cast<char>(0xBF) };

	logfile.write(bom, sizeof(bom));

	for (const auto &filename : fs.enumerate()) {
		logfile << filename << "\n";
	}

	auto file = fs.lookup("meshes\\x\\ex_hlaalu_b_01.nif");
	if (!file)
		throw std::logic_error("test file not found");

	std::ofstream dumpFile;
	dumpFile.exceptions(std::ios::failbit | std::ios::badbit | std::ios::eofbit);
	dumpFile.open("C:\\projects\\archiveparse\\dump.bin", std::ios::out | std::ios::trunc | std::ios::binary);

	std::vector<char> data(file->fileSize());
	file->readFile(data.data(), data.size(), 0);
	dumpFile.write(data.data(), data.size());
}
