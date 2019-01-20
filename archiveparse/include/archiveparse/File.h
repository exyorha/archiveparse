#ifndef ARCHIVEPARSE_FILE_H
#define ARCHIVEPARSE_FILE_H

namespace archiveparse {
	class File {
	protected:
		File();

	public:
		virtual ~File();

		File(const File &other) = delete;
		File &operator =(const File &other) = delete;
	};
}

#endif
