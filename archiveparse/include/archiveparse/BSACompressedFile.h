#ifndef ARCHIVEPARSE_BSA_COMPRESSED_FILE_H
#define ARCHIVEPARSE_BSA_COMPRESSED_FILE_H

#include <archiveparse/File.h>

#include <Windows.h>

#include <vector>

namespace archiveparse {
	class BSACompressedFile final : public File {
	public:
		BSACompressedFile(HANDLE handle, uint64_t offset, size_t size);
		virtual ~BSACompressedFile();

		BSACompressedFile(const BSACompressedFile &other) = delete;
		BSACompressedFile &operator =(const BSACompressedFile &other) = delete;

		virtual size_t fileSize() const override;
		virtual size_t readFile(void *buf, size_t size, size_t offset) override;

	private:
		std::vector<unsigned char> m_data;
	};
}

#endif
