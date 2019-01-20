#ifndef ARCHIVEPARSE_WINDOWS_HANDLE_H
#define ARCHIVEPARSE_WINDOWS_HANDLE_H

#include <memory>

namespace archiveparse {
	struct WindowsHandleDeleter {
		void operator()(void *handle) const;
	};

	using WindowsHandle = std::unique_ptr<void, WindowsHandleDeleter>;
}

#endif
