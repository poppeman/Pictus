#include "gif_stringtable.h"
#include "orz/geom.h"
#include "surface_locked_area.h"

namespace Img {
	using Geom::RectInt;
	using Geom::PointInt;

	StringTable::StringTable(ImageDescriptor::Ptr id, uint8_t minCodeSize, bool Interlace):
		m_id{ id },
		m_stride{ 0 },
		m_x{ 0 },
		m_y{ 0 },
		m_width{ 0 },
		m_height{ 0 },
		m_pass{ 0 },
		m_pos{ 0 },
		m_lastCode{ -1 },
		m_interlace{ Interlace },
		m_initialCodeSize{ minCodeSize },
		m_startWithClear{ false }
	{
		if (minCodeSize > 11) {
			DO_THROW(Err::InvalidParam, "StringTable::ctor: Invalid initial code size.");
		}

		m_clearCode = (1 << m_initialCodeSize);
		m_endCode = (m_clearCode + 1);

		resetTable();
	}

	void StringTable::resetTable() {
		m_maxCode = (short)(1 << m_initialCodeSize);
		m_stream.ChangeWordSize(static_cast<uint8_t>(m_initialCodeSize + 1));

		// Set each entry to only contain the root.
		for (int i = 0; i < LZWTableSize; i++) {
			m_table[i].resize(1, static_cast<uint8_t>(i));
		}

		m_numCodes = m_maxCode + 2;
	}

	int StringTable::writeString(short code, uint8_t* pDest, int pos) {
		EntryList& currentString = m_table[code];
		for (unsigned int i=0;i<currentString.size();i++) {
			if (m_x >= m_width || m_y >= m_height) {
				return pos;
			}

			if (!m_interlace) {
				pDest[m_x + m_y * m_stride] = static_cast<uint8_t>(currentString[i]);
				++m_x;
				if (m_x == m_width) {
					m_x = 0;
					++m_y;
				}
			}
			else {
				if (m_pass <=4) {
					pDest[m_x + m_y * m_stride] = static_cast<uint8_t>(currentString[i]);
					++m_x;
				}
				else {
					++m_x;
				}

				if (m_x == m_width) {
					m_x = 0;
					if (m_pass == 0) {
						// Every eight row for first pass
						m_y += 8;

						// Start with row 4 on second pass.
						if (m_y >= m_height) {
							m_y		= 4;
							m_pass	= 1;
						}
					}
					else if (m_pass == 1) {
						// Every eight row for first second
						m_y += 8;

						// Start with row 2 on second pass.
						if (m_y >= m_height) {
							m_y = 2;
							m_pass = 2;
						}
					}
					else if (m_pass == 2) {
						// Every fourth row for first third pass
						m_y += 4;

						// Start with row 1 on second pass.
						if (m_y >= m_height) {
							m_y		= 1;
							m_pass	= 3;
						}
					}
					else if (m_pass == 3) {
						// Every second row for final pass
						m_y += 2;
					}
				}
			}
		}
		return pos;
	}

	short StringTable::addString(short lastCode, short currentCode) {
		if (m_numCodes == LZWTableSize) {
			return m_numCodes; // Don't touch the table if it's full.
		}

		if (m_numCodes > 4095) {
			DO_THROW(Err::CodecError, "Code table size out of bounds.");
		}

		if (lastCode < 0 || currentCode < 0) {
			DO_THROW(Err::CodecError, "LZW stream invalid.");
		}

		// PERF: assigns eat performance, append only slightly so.
		m_table[m_numCodes++]	= m_table[lastCode] + m_table[currentCode][0];

		int codeSize			= m_stream.CurrentWordSize();

		if (m_numCodes >= (1<<codeSize)  && codeSize < 12) {
			m_stream.ChangeWordSize(codeSize+1);
			m_maxCode <<= 1;
		}
		return (short)(m_numCodes - 1);
	}

	bool StringTable::Decompress(uint8_t* pSource, size_t bufferSize) {
		m_stream.AddBytes(pSource, bufferSize);

		std::shared_ptr<Surface::LockedArea> area = m_id->Surface->LockSurface(RectInt(PointInt(0, 0), m_id->ImageSize));
		uint8_t* pDest = area->Buffer();
		m_stride	= area->Stride();
		m_width		= m_id->ImageSize.Width;
		m_height	= m_id->ImageSize.Height;

		if (m_startWithClear && m_stream.IsWordsLeft()) {
			resetTable();

			m_lastCode			= static_cast<short>(m_stream.GetWord());
			m_pos				= writeString(m_lastCode, pDest, m_pos);
			m_startWithClear	= false;
		}

		while(m_stream.IsWordsLeft()) {
			short currentCode = static_cast<short>(m_stream.GetWord());

			if (currentCode == m_endCode) {
				return false;
			}

			if (currentCode == m_clearCode) {
				if (m_stream.IsWordsLeft()) {
					resetTable();
					m_lastCode = static_cast<short>(m_stream.GetWord());
					m_pos = writeString(m_lastCode, pDest, m_pos);
				}
				else {
					m_startWithClear = true;
				}
			}
			else {
				if (m_numCodes>currentCode) {
					m_pos = writeString(currentCode, pDest, m_pos);
					if (m_lastCode != -1) addString(m_lastCode, currentCode);
					m_lastCode = currentCode;
				}
				else {
					short newCode	= addString(m_lastCode, m_lastCode);
					m_pos			= writeString(newCode, pDest, m_pos);
					m_lastCode		= currentCode;
				}
			}
		}
		return true;
	}
}
