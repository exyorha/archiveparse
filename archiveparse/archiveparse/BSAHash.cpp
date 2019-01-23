#include <archiveparse/BSAHash.h>

namespace archiveparse {
	MorrowindBSAFileNameHash calculateMorrowindHash(const std::string &filename) {
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

		MorrowindBSAFileNameHash hash;
		hash.val1 = value1;
		hash.val2 = sum;
		return hash;
	}

	uint64_t calculateCurrentHashWithExtension(const std::string &string) {
		uint64_t hash = 0;

		auto extensionDelimiter = std::find(string.rbegin(), string.rend(), '.');
		if (extensionDelimiter != string.rend()) {
			hash = currentHashString(extensionDelimiter.base() - 1, string.end(), hash);

			hash = calculateCurrentHash(string.begin(), extensionDelimiter.base() - 1, hash);
		}
		else {
			hash = calculateCurrentHash(string.begin(), string.end(), hash);
		}

		if (extensionDelimiter != string.rend()) {
			std::string extension(extensionDelimiter.base(), string.end());

			if (extension == "kf") {
				hash += 0x80ULL;
			}
			else if (extension == "nif") {
				hash += 0x8000ULL;
			}
			else if (extension == "dds") {
				hash += 0x8080ULL;
			}
			else if (extension == "wav") {
				hash += 0x80000000ULL;
			}
		}

		return hash;

	}
}