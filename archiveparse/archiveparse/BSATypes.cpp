#include <archiveparse/BSATypes.h>
#include <tuple>
#include <sstream>

namespace archiveparse {

	bool MorrowindBSAFileNameHash::operator < (const MorrowindBSAFileNameHash &other) const {
		return std::tie(val1, val2) < std::tie(other.val1, other.val2);
	}


	bool CurrentBSAFileNameHash::operator < (const CurrentBSAFileNameHash &other) const {
		return std::tie(val2, val1) < std::tie(other.val2, other.val1);
	}

	std::string CurrentBSAFileNameHash::toString() const {
		std::stringstream stream;
		stream << std::hex;
		stream.width(8);
		stream.fill('0');
		stream << val1;
		stream.width(8);
		stream.fill('0');
		stream << val2;
		return stream.str();
	}
}
