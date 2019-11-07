#include <archiveparse/EncodingUtilities.h>

#include <Windows.h>

#include <stdexcept>

namespace archiveparse {
	namespace impl {
		std::wstring multiByteToWideChar(unsigned int codepage, unsigned int flags, const std::string &string) {
			if (string.empty())
				return {};

			std::wstring outbuf;

			int result = MultiByteToWideChar(codepage, flags, string.data(), string.size(), outbuf.data(), outbuf.size());
			if (result == 0)
				throw std::runtime_error("MultiByteToWideChar failed");

			outbuf.resize(result);

			result = MultiByteToWideChar(codepage, flags, string.data(), string.size(), outbuf.data(), outbuf.size());
			if (result == 0)
				throw std::runtime_error("MultiByteToWideChar failed");

			return outbuf;
		}

		std::string wideCharToMultiByte(unsigned int codepage, unsigned int flags, const std::wstring &string) {
			if (string.empty())
				return {};

			std::string outbuf;

			int result = WideCharToMultiByte(codepage, flags, string.data(), string.size(), outbuf.data(), outbuf.size(), nullptr, nullptr);
			if (result == 0)
				throw std::runtime_error("WideCharToMultiByte failed");

			outbuf.resize(result);

			result = WideCharToMultiByte(codepage, flags, string.data(), string.size(), outbuf.data(), outbuf.size(), nullptr, nullptr);
			if (result == 0)
				throw std::runtime_error("WideCharToMultiByte failed");

			return outbuf;
		}
	}

	std::wstring utf8ToWide(const std::string &str) {
		return impl::multiByteToWideChar(CP_UTF8, 0, str);
	}

	std::string wideToUtf8(const std::wstring &str) {
		return impl::wideCharToMultiByte(CP_UTF8, 0, str);
	}

	std::wstring codepageToWide(const std::string &str, unsigned int codepage) {
		return impl::multiByteToWideChar(codepage, 0, str);
	}

	std::string wideToCodepage(const std::wstring &str, unsigned int codepage) {
		return impl::wideCharToMultiByte(codepage, 0, str);
	}

	std::string codepageToUtf8(const std::string &str, unsigned int codepage) {
		return wideToUtf8(codepageToWide(str, codepage));
	}

	std::string utf8ToCodepage(const std::string &str, unsigned int codepage) {
		return wideToCodepage(utf8ToWide(str), codepage);
	}
}
