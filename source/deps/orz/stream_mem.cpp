#include "stream_mem.h"
#include <algorithm>

namespace IO {
	size_t StreamMemory::performRead(void* buf, size_t size, size_t items) {
		std::lock_guard<std::recursive_mutex> l(m_mutexAccess);
		if(m_pos >= m_size) throw Err::CriticalError("Position was out of bounds, invalid object state.");
		auto numItems = std::min<size_t>(items, (m_size - m_pos) / size);
		memcpy(buf, m_data + m_pos, size * numItems);
		m_pos += size * numItems;
		return numItems;
	}

	FileInt StreamMemory::performPosition() const {
		return m_pos;
	}

	void StreamMemory::performClose() {
		std::lock_guard<std::recursive_mutex> l(m_mutexAccess);
		m_data = 0;
		m_pos = 0;
		m_size = 0;
		m_isOpen = false;
	}

	bool StreamMemory::performOpen() {
		std::lock_guard<std::recursive_mutex> l(m_mutexAccess);
		m_isOpen = (m_data != 0);
		m_pos = 0;
		return m_isOpen;
	}

	void StreamMemory::performSeek(FileInt position, SeekMethod m) {
		std::lock_guard<std::recursive_mutex> l(m_mutexAccess);

		if(m == SeekMethod::Begin)
			m_pos = position;
		else if (m == SeekMethod::Current)
			m_pos += position;
		else if (m == SeekMethod::End)
			m_pos = m_size;

		if(m_pos > m_size) m_pos = m_size;
	}

	bool StreamMemory::performIsOpen() const {
		return m_isOpen;
	}

	FileInt StreamMemory::performSize() {
		return m_size;
	}

	std::string StreamMemory::performName() const {
		return "Memory buffer";
	}

	StreamMemory::StreamMemory(uint8_t* data, size_t size) :m_data(data), m_size(size), m_pos(0), m_isOpen(false) {}
}
