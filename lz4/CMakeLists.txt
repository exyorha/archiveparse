add_library(lz4 STATIC 
	lz4/lib/lz4.c
	lz4/lib/lz4.h
	lz4/lib/lz4frame.c
	lz4/lib/lz4frame.h
	lz4/lib/lz4frame_static.h
	lz4/lib/lz4hc.c
	lz4/lib/lz4hc.h
	lz4/lib/xxhash.c
	lz4/lib/xxhash.h
)
target_include_directories(lz4 PUBLIC lz4/lib)

