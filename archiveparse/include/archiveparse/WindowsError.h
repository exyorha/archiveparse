#ifndef ARCHIVEPARSE_WINDOWS_ERROR_H
#define ARCHIVEPARSE_WINDOWS_ERROR_H

#include <stdexcept>
#include <Windows.h>

namespace archiveparse {
	class WindowsError final : public std::runtime_error {
	public:
		explicit WindowsError(DWORD error = GetLastError());
		virtual ~WindowsError() noexcept;

	private:
		static std::string errorToString(DWORD error);
	};
}

#endif
