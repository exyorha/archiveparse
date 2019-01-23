#ifndef ARCHIVEPARSE_BSATYPES_H
#define ARCHIVEPARSE_BSATYPES_H

#include <stdint.h>

#include <string>

namespace archiveparse {
	enum : uint32_t {
		MorrowindBSAVersion = 0x00000100,

		CurrentBSAMagic = 0x00415342,
		OblivionBSAVersion = 103,

		BSAFolderNamesRetained = 1,
		BSAFileNamesRetained = 2,
		BSACompressedByDefault = 4,
		BSAFileSizeFlagCompressed = 1U << 30,
		BSAFileSizeMask = (1U << 30) - 1
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

	struct MorrowindBSAFileNameHash {
		uint32_t val1;
		uint32_t val2;

		bool operator <(const MorrowindBSAFileNameHash &other) const;
	};

	struct CurrentBSAFileNameHash {
		inline CurrentBSAFileNameHash(uint64_t val = 0) {
			val1 = static_cast<uint32_t>(val);
			val2 = static_cast<uint32_t>(val >> 32);
		}

		uint32_t val1;
		uint32_t val2;

		bool operator <(const CurrentBSAFileNameHash &other) const;
		std::string toString() const;
	};

	struct CurrentBSAHeader {
		uint32_t magic;
		uint32_t version;
		uint32_t folderOffset;
		uint32_t archiveFlags;
		uint32_t folderCount;
		uint32_t fileCount;
		uint32_t totalFolderNameLength;
		uint32_t totalFileNameLength;
		uint32_t fileFlags;
	};

	struct CurrentBSAFolder {
		CurrentBSAFileNameHash nameHash;
		uint32_t count;
		uint32_t offset;
	};

	struct CurrentBSAFile {
		CurrentBSAFileNameHash nameHash;
		uint32_t size;
		uint32_t offset;
	};
}

#endif
