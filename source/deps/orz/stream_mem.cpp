#include "StdAfx.h"
#include "stream_mem.h"

namespace IO {
	_Check_return_ size_t StreamMemory::performRead(_Out_writes_bytes_(size * items) void* buf, _In_ size_t size, _In_ size_t items) {
		std::lock_guard<std::recursive_mutex> l(m_mutexAccess);
		if(m_pos >= m_size) throw Err::CriticalError(L"Position was out of bounds, invalid object state.");
		auto numItems = Util::Min<size_t>(items, (m_size - m_pos) / size);
		memcpy(buf, m_data + m_pos, size * numItems);
		m_pos += size * numItems;
		return numItems;
	}

	_Check_return_ FileInt StreamMemory::performPosition() const {
		return m_pos;
	}

	void StreamMemory::performClose() {
		std::lock_guard<std::recursive_mutex> l(m_mutexAccess);
		m_data = 0;
		m_pos = 0;
		m_size = 0;
		m_isOpen = false;
	}

	_Check_return_ bool StreamMemory::performOpen() {
		std::lock_guard<std::recursive_mutex> l(m_mutexAccess);
		m_isOpen = (m_data != 0);
		m_pos = 0;
		return m_isOpen;
	}

	void StreamMemory::performSeek(_In_ FileInt position, _In_ SeekMethod m) {
		std::lock_guard<std::recursive_mutex> l(m_mutexAccess);

		if(m == SeekMethod::Begin)
			m_pos = position;
		else if (m == SeekMethod::Current)
			m_pos += position;
		else if (m == SeekMethod::End)
			m_pos = m_size;

		if(m_pos > m_size) m_pos = m_size;
	}

	_Check_return_ bool StreamMemory::performIsOpen() const {
		return m_isOpen;
	}

	_Check_return_ FileInt StreamMemory::performSize() {
		return m_size;
	}

	_Check_return_ std::wstring StreamMemory::performName() const {
		return TX("Memory buffer");
	}

	StreamMemory::StreamMemory(_In_reads_bytes_(size) uint8_t* data, _In_ size_t size) :m_data(data), m_size(size), m_pos(0), m_isOpen(false) {}
}
