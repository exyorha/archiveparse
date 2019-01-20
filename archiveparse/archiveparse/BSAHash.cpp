#include <archiveparse/BSAHash.h>

namespace archiveparse {
	MorrowindFileNameHash calculateMorrowindHash(const std::string &filename) {
		size_t length = filename.size();
		unsigned int sum = 0;
		unsigned int off = 0;
		unsigned int pos;

		for (pos = 0; pos < length / 2; pos++) {
			sum ^= filename[pos] << (off & 31);
			off += 8;
		}

		unsigned int value1 = sum;

		sum = 0;
		off = 0;
		for (; pos < length; pos++) {
			unsigned int val = filename[pos] << (off & 31);
			sum ^= val;
			unsigned int rot = val & 31;
			sum = (sum << (32 - rot)) | (sum >> rot);
			off += 8;
		}

		MorrowindFileNameHash hash;
		hash.val1 = value1;
		hash.val2 = sum;
		return hash;
	}
}