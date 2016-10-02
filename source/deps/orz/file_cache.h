#ifndef ORZ_FILE_CACHE_H
#define ORZ_FILE_CACHE_H

#include "file_reader.h"

namespace IO {
	class FileReaderByteStreamer {
	public:
		void SetFileReader(IO::FileReader::Ptr file);
		uint8_t& ReadByte();
		FileInt BytesLeft();
		void SkipBytes(int numToSkip);
		FileInt VirtualFilePosition();

	private:
		enum {
			CacheSize = 4096
		};

		uint8_t m_cache[CacheSize] = {};
		size_t m_position = 0;
		size_t m_cacheSize = 0;
		FileInt m_bytesLeft = 0;
		IO::FileReader::Ptr m_reader;
	};
}

#endif
