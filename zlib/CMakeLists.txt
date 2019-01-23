add_library(zlib STATIC EXCLUDE_FROM_ALL
    adler32.c
    crc32.c
    crc32.h
    deflate.c
    deflate.h
    infback.c
    inffast.c
    inffast.h
    inffixed.h
    inflate.c
    inflate.h
    inftrees.c
    inftrees.h
    trees.c
    trees.h
    zutil.c
    zutil.h
    include/zconf.h
    include/zlib.h
)
target_include_directories(zlib PUBLIC include)
