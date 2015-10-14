#ifndef ORZ_STREAM_FILE_H
#define ORZ_STREAM_FILE_H

#include "stream.h"

#include <mutex>
#ifdef _WIN32
#include <windows.h>
#endif

namespace IO {
	class StreamFile:public Stream {
	public:
		StreamFile(const std::string& filename);
		~StreamFile();

#ifdef _WIN32
		std::string Rename(const std::string& newFilename, HWND handle);
		void Renamed(const std::string& newFilename);
		bool Delete(bool doRecycle, HWND handle);
#endif

	private:
		std::string performName() const;
		bool performOpen();
		bool performIsOpen() const;
		void performClose();
		size_t performRead(void* buf, size_t size, size_t items);
		void performSeek(FileInt position, SeekMethod m);
		FileInt performPosition() const;
		FileInt performSize();

		mutable std::recursive_mutex m_mutexAccess;

		std::FILE* m_file;
		std::string m_name;
		FileInt m_size;
		OpenErrorCode m_error;
	};
}

#endif
