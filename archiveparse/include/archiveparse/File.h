#ifndef ARCHIVEPARSE_FILE_H
#define ARCHIVEPARSE_FILE_H

#include <stdint.h>

namespace archiveparse {
	class File {
	protected:
		File();

	public:
		virtual ~File();

		File(const File &other) = delete;
		File &operator =(const File &other) = delete;

		virtual size_t fileSize() const = 0;
		virtual size_t readFile(void *buf, size_t size, size_t offset) = 0;
	};
}

#endif
