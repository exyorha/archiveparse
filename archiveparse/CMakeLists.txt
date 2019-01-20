add_library(archiveparse STATIC
	include/archiveparse/BSAFilesystemLayer.h
	include/archiveparse/BSATypes.h
	include/archiveparse/EncodingUtilities.h
	include/archiveparse/File.h
	include/archiveparse/Filesystem.h
	include/archiveparse/FilesystemLayer.h
	include/archiveparse/FilesystemLayerFactory.h
	include/archiveparse/LooseFilesFilesystemLayer.h
	include/archiveparse/WindowsHandle.h
	archiveparse/BSAFilesystemLayer.cpp
	archiveparse/EncodingUtilities.cpp
	archiveparse/File.cpp
	archiveparse/Filesystem.cpp
	archiveparse/FilesystemLayer.cpp
	archiveparse/FilesystemLayerFactory.cpp
	archiveparse/LooseFilesFilesystemLayer.cpp
	archiveparse/WindowsHandle.cpp
)
target_include_directories(archiveparse PUBLIC include)
target_compile_definitions(archiveparse PRIVATE -DUNICODE -D_UNICODE)
