#include <archiveparse/WindowsError.h>
#include <archiveparse/EncodingUtilities.h>

#include <sstream>

namespace archiveparse {
	WindowsError::WindowsError(DWORD error) : std::runtime_error(errorToString(error)) {

	}

	WindowsError::~WindowsError() noexcept = default;

	std::string WindowsError::errorToString(DWORD error) {
		struct PointerReleaser {
			PointerReleaser() : ptr(nullptr) {

			}

			~PointerReleaser() {
				if(ptr)
					LocalFree(ptr);
			}

			LPWSTR ptr;

		} ptr;

		auto result = FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			nullptr,
			error,
			0,
			reinterpret_cast<LPWSTR>(&ptr.ptr),
			0,
			nullptr);
				
		if (result == 0) {
			std::stringstream stream;
			stream << "Windows error " << result;
			return stream.str();
		}

		return wideToUtf8(std::wstring(ptr.ptr, result));
	}
}
