#ifndef ARCHIVEPARSE_ENCODING_UTILITIES_H
#define ARCHIVEPARSE_ENCODING_UTILITIES_H

#include <string>

namespace archiveparse {
	std::wstring utf8ToWide(const std::string &str);
	std::string wideToUtf8(const std::wstring &str);

	std::wstring codepageToWide(const std::string &str, unsigned int codepage);
	std::string wideToCodepage(const std::wstring &str, unsigned int codepage);

	std::string codepageToUtf8(const std::string &str, unsigned int codepage);
	std::string utf8ToCodepage(const std::string &str, unsigned int codepage);
}

#endif
