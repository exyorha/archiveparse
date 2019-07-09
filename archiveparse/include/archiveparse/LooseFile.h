#ifndef ARCHIVEPARSE_LOOSE_FILE_H
#define ARCHIVEPARSE_LOOSE_FILE_H

#include <archiveparse/File.h>
#include <archiveparse/WindowsHandle.h>

namespace archiveparse {
	class LooseFile final : public File {
	public:
		explicit LooseFile(WindowsHandle&& handle);
		virtual ~LooseFile();

		virtual size_t fileSize() const override;
		virtual size_t readFile(void* buf, size_t size, size_t offset) override;
	private:
		WindowsHandle m_handle;
	};
}

#endif
