#ifndef ARCHIVEPARSE_BSA_FILE_H
#define ARCHIVEPARSE_BSA_FILE_H

#include <archiveparse/File.h>

#include <Windows.h>

namespace archiveparse {
	class BSAFile final : public File {
	public:
		BSAFile(HANDLE handle, uint64_t offset, size_t size);
		virtual ~BSAFile();

		BSAFile(const BSAFile &other) = delete;
		BSAFile &operator =(const BSAFile &other) = delete;

		virtual size_t fileSize() const override;
		virtual size_t readFile(void *buf, size_t size, size_t offset) override;

	private:
		HANDLE m_file;
		uint64_t m_offset;
		size_t m_size;
	};
}

#endif
