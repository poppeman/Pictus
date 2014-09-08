#ifndef STREAM_FILE_H
#define STREAM_FILE_H

#include "stream.h"

namespace IO {
	class StreamFile:public Stream {
	public:
		StreamFile(_In_ const std::wstring& filename);
		~StreamFile();

#ifdef WIN32
		std::wstring Rename(_In_ const std::wstring& newFilename, _In_ HWND handle);
		void Renamed(_In_ const std::wstring& newFilename);
		_Check_return_ bool Delete(_In_ bool doRecycle, _In_ HWND handle);
#endif

	private:
		_Check_return_ std::wstring performName() const;
		_Check_return_ bool performOpen();
		_Check_return_ bool performIsOpen() const;
		void performClose();
		_Check_return_ size_t performRead(_Out_writes_bytes_(size * items) void* buf, _In_ size_t size, _In_ size_t items);
		void performSeek(_In_ FileInt position, _In_ SeekMethod m);
		_Check_return_ FileInt performPosition() const;
		_Check_return_ FileInt performSize();

		mutable std::recursive_mutex m_mutexAccess;

		std::FILE* m_file;
		std::wstring m_name;
		FileInt m_size;
		OpenErrorCode m_error;
	};
}

#endif
