cmake_minimum_required(VERSION 3.12)
project(archiveparse)

if(${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME})
	set(CMAKE_CXX_STANDARD 17)
	set(CMAKE_CXX_STANDARD_REQUIRED ON)
	set(CMAKE_CXX_EXTENSIONS OFF)
endif()

add_subdirectory(zlib)
add_subdirectory(archiveparse)

if(${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME})
	add_subdirectory(archiveparse-test)
endif()
