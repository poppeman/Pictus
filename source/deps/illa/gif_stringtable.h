
#ifndef GIF_STRINGTABLE_H
#define GIF_STRINGTABLE_H

#include "orz/streamconv.h"
#include "gif_imagedescriptor.h"

namespace Img {
	class StringTable {
	public:
		bool Decompress(uint8_t* pSource, size_t bufferSize);

		StringTable(ImageDescriptor::Ptr id, uint8_t minCodeSize, bool Interlace);

	private:
		int writeString(short code, uint8_t* pDest, int pos);
		short addString(short lastCode, short currentCode);

	private:
		enum {
			LZWTableSize		= 4096,
		};

	private:
		// Destination image data
		ImageDescriptor::Ptr m_id;

		size_t m_stride;
		int m_x, m_y, m_width, m_height, m_pass, m_pos;
		short m_lastCode;
		bool m_interlace;

		uint8_t m_initialCodeSize;
		short m_clearCode;
		short m_endCode;
		short m_maxCode;

		Util::StreamConverter m_stream;
		short m_numCodes;

		typedef std::basic_string<uint8_t> EntryList;
		EntryList m_table[LZWTableSize];

		bool m_startWithClear;

		void resetTable();
	};
}

#endif
