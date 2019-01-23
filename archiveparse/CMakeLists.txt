add_library(archiveparse STATIC
	include/archiveparse/BSACompressedFile.h
	include/archiveparse/BSAFile.h
	include/archiveparse/BSAFilesystemLayer.h
	include/archiveparse/BSAHash.h
	include/archiveparse/BSATypes.h
	include/archiveparse/EncodingUtilities.h
	include/archiveparse/File.h
	include/archiveparse/Filesystem.h
	include/archiveparse/FilesystemLayer.h
	include/archiveparse/FilesystemLayerFactory.h
	include/archiveparse/LooseFilesFilesystemLayer.h
	include/archiveparse/WindowsHandle.h
	archiveparse/BSACompressedFile.cpp
	archiveparse/BSAFile.cpp
	archiveparse/BSAFilesystemLayer.cpp
	archiveparse/BSAHash.cpp
	archiveparse/BSATypes.cpp
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
target_link_libraries(archiveparse PRIVATE zlib lz4)
