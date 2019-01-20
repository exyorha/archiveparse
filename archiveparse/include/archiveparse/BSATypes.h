#ifndef ARCHIVEPARSE_BSATYPES_H
#define ARCHIVEPARSE_BSATYPES_H

#include <stdint.h>

namespace archiveparse {
	enum : uint32_t {
		MorrowindBSAVersion = 0x00000100
	};

	struct MorrowindBSAHeader {
		uint32_t version;
		uint32_t hashOffset;
		uint32_t fileCount;
	};

	struct MorrowindFileSizeAndOffset {
		uint32_t fileSize;
		uint32_t fileOffset;
	};

	struct MorrowindFileNameHash {
		uint32_t val1;
		uint32_t val2;
	};
}

#endif
