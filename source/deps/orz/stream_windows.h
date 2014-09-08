#ifndef STREAMWIN_H
#define STREAMWIN_H

#include "stream.h"

namespace IO {
	class StreamWindows:public Stream {
	public:
		StreamWindows(_In_ IStream* i);
		StreamWindows(_In_ const std::wstring& filename);
		~StreamWindows();

	private:
		_Check_return_ std::wstring performName() const;
		_Check_return_ bool performOpen();
		_Check_return_ bool performIsOpen() const;
		void performClose();
		_Check_return_ size_t performRead(_Out_writes_bytes_(size * items) void* buf, _In_ size_t size, _In_ size_t items);
		void performSeek(_In_ FileInt position, _In_ SeekMethod m);
		_Check_return_ FileInt performPosition() const;
		_Check_return_ FileInt performSize();

		IStream* m_pStream;
		std::wstring m_filename;
	};
}

#endif
