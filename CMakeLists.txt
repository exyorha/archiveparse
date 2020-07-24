cmake_minimum_required(VERSION 3.12)
project(archiveparse)

add_subdirectory(zlib)
add_subdirectory(lz4)
add_subdirectory(archiveparse)

if(${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME})
	add_subdirectory(archiveparse-test)
endif()
