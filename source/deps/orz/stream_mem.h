#ifndef ORZ_STREAM_MEM_H
#define ORZ_STREAM_MEM_H

#include "stream.h"
#include <mutex>

namespace IO {
	class StreamMemory:public Stream {
	public:
		StreamMemory(uint8_t* buffer, size_t size);

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

		uint8_t* m_data;
		FileInt m_size;
		FileInt m_pos;
		bool m_isOpen;
	};
}

#endif
