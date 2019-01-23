#include <archiveparse/Filesystem.h>
#include <archiveparse/FilesystemLayer.h>
#include <archiveparse/FilesystemLayerFactory.h>
#include <archiveparse/File.h>

#include <fstream>

int main(int argc, char **argv) {
	archiveparse::Filesystem fs;
	archiveparse::FilesystemLayerFactory factory;
	//fs.addLayer(factory.createLayer("BSA", "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Morrowind\\Data Files\\Morrowind.bsa", {}));
	//fs.addLayer(factory.createLayer("BSA", "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Morrowind\\Data Files\\Bloodmoon.bsa", {}));
	//fs.addLayer(factory.createLayer("BSA", "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Morrowind\\Data Files\\Tribunal.bsa", {}));
	//fs.addLayer(factory.createLayer("LooseFiles", "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Morrowind\\Data Files", {}));

	//fs.addLayer(factory.createLayer("BSA", "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Oblivion\\Data\\Oblivion - Meshes.bsa", {}));
	//fs.addLayer(factory.createLayer("BSA", "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Oblivion\\Data\\Oblivion - Misc.bsa", {}));
	//fs.addLayer(factory.createLayer("BSA", "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Oblivion\\Data\\Oblivion - Sounds.bsa", {}));
	//fs.addLayer(factory.createLayer("BSA", "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Oblivion\\Data\\Oblivion - Textures - Compressed.bsa", {}));
	//fs.addLayer(factory.createLayer("BSA", "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Oblivion\\Data\\Oblivion - Voices1.bsa", {}));
	//fs.addLayer(factory.createLayer("BSA", "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Oblivion\\Data\\Oblivion - Voices2.bsa", {}));
	//fs.addLayer(factory.createLayer("BSA", "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Oblivion\\Data\\DLCShiveringIsles - Meshes.bsa", {}));
	//fs.addLayer(factory.createLayer("BSA", "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Oblivion\\Data\\DLCShiveringIsles - Sounds.bsa", {}));
	//fs.addLayer(factory.createLayer("BSA", "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Oblivion\\Data\\DLCShiveringIsles - Textures.bsa", {}));
	//fs.addLayer(factory.createLayer("BSA", "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Oblivion\\Data\\DLCShiveringIsles - Voices.bsa", {}));
	//fs.addLayer(factory.createLayer("BSA", "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Oblivion\\Data\\Knights.bsa", {}));
	
	fs.addLayer(factory.createLayer("BSA", "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Skyrim\\Data\\Skyrim - Animations.bsa", {}));
	fs.addLayer(factory.createLayer("BSA", "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Skyrim\\Data\\Skyrim - Interface.bsa", {}));
	fs.addLayer(factory.createLayer("BSA", "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Skyrim\\Data\\Skyrim - Meshes.bsa", {}));
	fs.addLayer(factory.createLayer("BSA", "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Skyrim\\Data\\Skyrim - Misc.bsa", {}));
	fs.addLayer(factory.createLayer("BSA", "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Skyrim\\Data\\Skyrim - Shaders.bsa", {}));
	fs.addLayer(factory.createLayer("BSA", "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Skyrim\\Data\\Skyrim - Sounds.bsa", {}));
	fs.addLayer(factory.createLayer("BSA", "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Skyrim\\Data\\Skyrim - Voices.bsa", {}));
	fs.addLayer(factory.createLayer("BSA", "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Skyrim\\Data\\Skyrim - VoicesExtra.bsa", {}));
	fs.addLayer(factory.createLayer("BSA", "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Skyrim\\Data\\Update.bsa", {}));
	fs.addLayer(factory.createLayer("BSA", "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Skyrim\\Data\\Dawnguard.bsa", {}));
	fs.addLayer(factory.createLayer("BSA", "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Skyrim\\Data\\Dragonborn.bsa", {}));
	fs.addLayer(factory.createLayer("BSA", "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Skyrim\\Data\\HearthFires.bsa", {}));
	fs.addLayer(factory.createLayer("BSA", "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Skyrim\\Data\\HighResTexturePack01.bsa", {}));
	fs.addLayer(factory.createLayer("BSA", "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Skyrim\\Data\\HighResTexturePack02.bsa", {}));
	fs.addLayer(factory.createLayer("BSA", "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Skyrim\\Data\\HighResTexturePack03.bsa", {}));
	
	/*
	fs.addLayer(factory.createLayer("BSA", "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Skyrim Special Edition\\Data\\Skyrim - Animations.bsa", {}));
	fs.addLayer(factory.createLayer("BSA", "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Skyrim Special Edition\\Data\\Skyrim - Interface.bsa", {}));
	fs.addLayer(factory.createLayer("BSA", "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Skyrim Special Edition\\Data\\Skyrim - Meshes0.bsa", {}));
	fs.addLayer(factory.createLayer("BSA", "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Skyrim Special Edition\\Data\\Skyrim - Meshes1.bsa", {}));
	fs.addLayer(factory.createLayer("BSA", "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Skyrim Special Edition\\Data\\Skyrim - Misc.bsa", {}));
	fs.addLayer(factory.createLayer("BSA", "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Skyrim Special Edition\\Data\\Skyrim - Patch.bsa", {}));
	fs.addLayer(factory.createLayer("BSA", "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Skyrim Special Edition\\Data\\Skyrim - Shaders.bsa", {}));
	fs.addLayer(factory.createLayer("BSA", "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Skyrim Special Edition\\Data\\Skyrim - Sounds.bsa", {}));
	fs.addLayer(factory.createLayer("BSA", "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Skyrim Special Edition\\Data\\Skyrim - Textures0.bsa", {}));
	fs.addLayer(factory.createLayer("BSA", "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Skyrim Special Edition\\Data\\Skyrim - Textures1.bsa", {}));
	fs.addLayer(factory.createLayer("BSA", "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Skyrim Special Edition\\Data\\Skyrim - Textures2.bsa", {}));
	fs.addLayer(factory.createLayer("BSA", "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Skyrim Special Edition\\Data\\Skyrim - Textures3.bsa", {}));
	fs.addLayer(factory.createLayer("BSA", "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Skyrim Special Edition\\Data\\Skyrim - Textures4.bsa", {}));
	fs.addLayer(factory.createLayer("BSA", "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Skyrim Special Edition\\Data\\Skyrim - Textures5.bsa", {}));
	fs.addLayer(factory.createLayer("BSA", "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Skyrim Special Edition\\Data\\Skyrim - Textures6.bsa", {}));
	fs.addLayer(factory.createLayer("BSA", "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Skyrim Special Edition\\Data\\Skyrim - Textures7.bsa", {}));
	fs.addLayer(factory.createLayer("BSA", "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Skyrim Special Edition\\Data\\Skyrim - Textures8.bsa", {}));
	fs.addLayer(factory.createLayer("BSA", "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Skyrim Special Edition\\Data\\Skyrim - Voices_en0.bsa", {}));
	*/
	std::ofstream logfile;
	logfile.exceptions(std::ios::failbit | std::ios::badbit | std::ios::eofbit);
	logfile.open("C:\\projects\\archiveparse\\log.txt", std::ios::out | std::ios::trunc);

	static const char bom[] = { static_cast<char>(0xEF), static_cast<char>(0xBB), static_cast<char>(0xBF) };

	logfile.write(bom, sizeof(bom));

	for (const auto &filename : fs.enumerate()) {
		logfile << filename << "\n";
	}

	//auto file = fs.lookup("meshes\\x\\ex_hlaalu_b_01.nif");
	//auto file = fs.lookup("textures\\architecture\\anvil\\altarcloth.dds");
	//auto file = fs.lookup("trees\\treecottonwoodsu.spt");
	//auto file = fs.lookup("meshes\\dungeons\\imperial\\largehall\\implhalldetwist02.nif");
	auto file = fs.lookup("textures\\actors\\elk\\elkbody.dds");
	if (!file)
		throw std::logic_error("test file not found");

	std::ofstream dumpFile;
	dumpFile.exceptions(std::ios::failbit | std::ios::badbit | std::ios::eofbit);
	dumpFile.open("C:\\projects\\archiveparse\\dump.bin", std::ios::out | std::ios::trunc | std::ios::binary);

	std::vector<char> data(file->fileSize());
	file->readFile(data.data(), data.size(), 0);
	dumpFile.write(data.data(), data.size());
}
