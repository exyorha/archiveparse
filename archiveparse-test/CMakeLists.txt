add_executable(archiveparse-test
	main.cpp
)

target_link_libraries(archiveparse-test PRIVATE archiveparse)
set_target_properties(archiveparse-test PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED TRUE CXX_EXTENSIONS OFF)
