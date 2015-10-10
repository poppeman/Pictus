#ifndef ORZ_STREAMWIN_H
#define ORZ_STREAMWIN_H

#include "stream.h"

#include <windows.h>
#include <Shlwapi.h>

namespace IO {
	class StreamWindows:public Stream {
	public:
		StreamWindows(IStream* i);
		StreamWindows(const std::wstring& filename);
		~StreamWindows();

	private:
		std::wstring performName() const;
		bool performOpen();
		bool performIsOpen() const;
		void performClose();
		size_t performRead(void* buf, size_t size, size_t items);
		void performSeek(FileInt position, SeekMethod m);
		FileInt performPosition() const;
		FileInt performSize();

		IStream* m_pStream;
		std::wstring m_filename;
	};
}

#endif
