#ifndef ARCHIVEPARSE_BSA_HASH_H
#define ARCHIVEPARSE_BSA_HASH_H

#include <archiveparse/BSATypes.h>
#include <string>

namespace archiveparse {
	MorrowindFileNameHash calculateMorrowindHash(const std::string &filename);
}

#endif
