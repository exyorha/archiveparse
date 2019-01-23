#include <archiveparse/BSACompressedFile.h>

#include <vector>

#include <zlib.h>

namespace archiveparse {
	static void *zlibAlloc(void *opaque, unsigned int items, unsigned int size) {
		(void)opaque;
		return malloc(items * size);
	}

	void zlibFree(void *opaque, void *address) {
		(void)opaque;
		free(address);
	}

	BSACompressedFile::BSACompressedFile(HANDLE handle, uint64_t offset, size_t size) {
		std::vector<unsigned char> compressedData(size);

		DWORD bytesRead;
		OVERLAPPED overlapped;
		ZeroMemory(&overlapped, sizeof(overlapped));
		overlapped.Offset = static_cast<DWORD>(offset);
		overlapped.OffsetHigh = static_cast<DWORD>(offset >> 32);

		if (!ReadFile(handle, compressedData.data(), size, &bytesRead, &overlapped) || bytesRead != size)
			throw std::runtime_error("ReadFile failed for compressed file");

		uint32_t uncompressedLength = compressedData[0] | (compressedData[1] << 8) | (compressedData[2] << 16) | (compressedData[3] << 24);

		m_data.resize(uncompressedLength);

		z_stream stream;
		memset(&stream, 0, sizeof(stream));
		stream.next_in = compressedData.data() + 4;
		stream.avail_in = compressedData.size() - 4;
		stream.next_out = m_data.data();
		stream.avail_out = m_data.size();

		stream.zalloc = zlibAlloc;
		stream.zfree = zlibFree;
		if (inflateInit(&stream) != Z_OK)
			throw std::runtime_error("inflateInit failed");

		if (inflate(&stream, Z_FINISH) != Z_STREAM_END) {
			inflateEnd(&stream);
			throw std::runtime_error("inflate failed");
		}

		inflateEnd(&stream);
	}

	BSACompressedFile::~BSACompressedFile() {

	}

	size_t BSACompressedFile::fileSize() const {
		return m_data.size();
	}
	
	size_t BSACompressedFile::readFile(void *buf, size_t size, size_t offset) {
		if (offset >= m_data.size())
			return 0;

		if (size + offset > m_data.size())
			size = m_data.size() - offset;

		memcpy(buf, m_data.data() + offset, size);

		return size;
	}
}
