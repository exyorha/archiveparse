#include <archiveparse/BSATypes.h>
#include <tuple>

namespace archiveparse {

	bool MorrowindFileNameHash::operator < (const MorrowindFileNameHash &other) const {
		return std::tie(val1, val2) < std::tie(other.val1, other.val2);
	}

}
