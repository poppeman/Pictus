#ifndef ORZ_STREAM_MEM_H
#define ORZ_STREAM_MEM_H

#include "stream.h"
#include <mutex>

namespace IO {
	class StreamMemory:public Stream {
	public:
		StreamMemory(_In_reads_bytes_(size) uint8_t* buffer, _In_ size_t size);

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

		uint8_t* m_data;
		FileInt m_size;
		FileInt m_pos;
		bool m_isOpen;
	};
}

#endif
