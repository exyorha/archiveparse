#include <archiveparse/WindowsHandle.h>

#include <Windows.h>

namespace archiveparse {
	void WindowsHandleDeleter::operator()(void *handle) const {
		CloseHandle(handle);
	}
}