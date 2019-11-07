#include <archiveparse/LooseFile.h>
#include <archiveparse/WindowsError.h>

#include <Windows.h>

namespace archiveparse {
	LooseFile::LooseFile(WindowsHandle&& handle) : m_handle(std::move(handle)) {

	}

	LooseFile::~LooseFile() = default;

	size_t LooseFile::fileSize() const {
		LARGE_INTEGER size;
		if (!GetFileSizeEx(m_handle.get(), &size))
			throw WindowsError();

		return static_cast<size_t>(size.QuadPart);
	}

	size_t LooseFile::readFile(void* buf, size_t size, size_t offset) {
		OVERLAPPED request;
		ZeroMemory(&request, sizeof(request));

		request.Offset = static_cast<DWORD>(offset);
		if constexpr (sizeof(offset) > sizeof(DWORD)) {
			request.OffsetHigh = static_cast<DWORD>(offset >> 32U);
		}

		DWORD bytesRead;
		if (!ReadFile(m_handle.get(), buf, static_cast<DWORD>(size), &bytesRead, &request))
			throw WindowsError();

		return bytesRead;		
	}
}
