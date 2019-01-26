#include <archiveparse/BSACompressedFile.h>
#include <archiveparse/WindowsError.h>

#include <vector>
#include <memory>

#include <zlib.h>
#include <lz4frame.h>

namespace archiveparse {
	static void *zlibAlloc(void *opaque, unsigned int items, unsigned int size) {
		(void)opaque;
		return malloc(items * size);
	}

	void zlibFree(void *opaque, void *address) {
		(void)opaque;
		free(address);
	}

	struct LZ4ContextDeleter {
		void operator()(LZ4F_dctx *ctx) {
			LZ4F_freeDecompressionContext(ctx);
		}
	};

	BSACompressedFile::BSACompressedFile(HANDLE handle, uint64_t offset, size_t size, Algorithm algorithm) {
		std::vector<unsigned char> compressedData(size);

		DWORD bytesRead;
		OVERLAPPED overlapped;
		ZeroMemory(&overlapped, sizeof(overlapped));
		overlapped.Offset = static_cast<DWORD>(offset);
		overlapped.OffsetHigh = static_cast<DWORD>(offset >> 32);

		if (!ReadFile(handle, compressedData.data(), size, &bytesRead, &overlapped))
			throw WindowsError();

		if (bytesRead != size)
			throw std::runtime_error("short read");

		uint32_t uncompressedLength = compressedData[0] | (compressedData[1] << 8) | (compressedData[2] << 16) | (compressedData[3] << 24);

		m_data.resize(uncompressedLength);

		if (algorithm == Algorithm::Zlib) {
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
		else if (algorithm == Algorithm::LZ4) {
			std::unique_ptr<LZ4F_dctx, LZ4ContextDeleter> context;
			LZ4F_dctx *rawCtx;
			if (LZ4F_isError(LZ4F_createDecompressionContext(&rawCtx, LZ4F_VERSION))) {
				throw std::logic_error("LZ4F_createDecompressionContext failed");
			}

			context.reset(rawCtx);

			auto outPtr = m_data.data();
			auto outEnd = outPtr + m_data.size();

			auto inPtr = compressedData.data() + 4;
			auto inEnd = inPtr + compressedData.size() - 4;

			while (outPtr < outEnd || inPtr < inEnd) {
				size_t inSize = inEnd - inPtr;
				size_t outSize = outEnd - outPtr;

				LZ4F_decompress(context.get(), outPtr, &outSize, inPtr, &inSize, nullptr);

				inPtr += inSize;
				outPtr += outSize;

				if (inSize == 0 && outSize == 0)
					throw std::logic_error("LZ4F_decompress failed");

			}
		}
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
