#include "StdAfx.h"
#include "file_cache.h"

namespace IO {
	FileReaderByteStreamer::FileReaderByteStreamer()
		:m_position(0),
		 m_cacheSize(0),
		 m_bytesLeft(0)
	{}

	void FileReaderByteStreamer::SetFileReader( IO::FileReader::Ptr file ) throw() {
		m_reader = file;
		m_bytesLeft = file->Size() - file->Position();
		m_position = m_cacheSize = 0;
	}

	uint8_t& FileReaderByteStreamer::ReadByte() {
		if (m_bytesLeft == 0) {
			DO_THROW(Err::CriticalError, L"No bytes left to fetch.");
		}

		if (m_position == m_cacheSize) {
			m_position = 0;
			m_cacheSize = m_reader->Read(m_cache, 1, CacheSize);
		}

		m_bytesLeft--;
		return m_cache[m_position++];
	}

	FileInt FileReaderByteStreamer::BytesLeft() {
		return m_bytesLeft;
	}

	void FileReaderByteStreamer::SkipBytes( int numToSkip ) {
		for (int i = 0; i < numToSkip; ++i)
			ReadByte();
	}

	FileInt FileReaderByteStreamer::VirtualFilePosition() {
		return m_reader->Position() - (m_cacheSize - m_position);
	}
}
