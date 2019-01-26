#include <archiveparse/BSAFile.h>
#include <archiveparse/WindowsError.h>

namespace archiveparse {
	BSAFile::BSAFile(HANDLE handle, uint64_t offset, size_t size) : m_file(handle), m_offset(offset), m_size(size) {

	}

	BSAFile::~BSAFile() {

	}

	size_t BSAFile::fileSize() const {
		return m_size;
	}

	size_t BSAFile::readFile(void *buf, size_t size, size_t offset) {
		if (offset >= m_size)
			return 0;

		if (size + offset > m_size)
			size = m_size - offset;

		DWORD bytesRead;
		OVERLAPPED overlapped;

		auto fullOffset = m_offset + offset;

		ZeroMemory(&overlapped, sizeof(overlapped));
		overlapped.Offset = static_cast<DWORD>(fullOffset);
		overlapped.OffsetHigh = static_cast<DWORD>(fullOffset >> 32);

		if (!ReadFile(m_file, buf, static_cast<DWORD>(size), &bytesRead, &overlapped))
			throw WindowsError();

		return bytesRead;
	}
}
