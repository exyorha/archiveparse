#ifndef ARCHIVEPARSE_BSA_HASH_H
#define ARCHIVEPARSE_BSA_HASH_H

#include <archiveparse/BSATypes.h>
#include <string>

namespace archiveparse {
	MorrowindBSAFileNameHash calculateMorrowindHash(const std::string &filename);

	template<typename It>
	uint64_t currentHashString(It begin, It end, uint64_t initial) {
		uint64_t hash = initial;

		for (auto it = begin; it != end; it++) {
			hash = (hash * 0x1003f) + *it;
		}

		return hash;
	}

	template<typename It>
	uint64_t calculateCurrentHash(It begin, It end, uint64_t initial) {
		auto hash = (initial + static_cast<uint32_t>(currentHashString(begin + 1, end - 2, 0))) << 32;

		uint32_t hash2 = 0;
		hash2 = *(end - 1);

		if (end - begin > 2) {
			hash2 |= *(end - 2) << 8;
		}

		hash2 |= ((end - begin) & 0xFF) << 16;
		hash2 |= (*begin) << 24;

		return hash + hash2;
	}

	uint64_t calculateCurrentHashWithExtension(const std::string &string);
}

#endif
