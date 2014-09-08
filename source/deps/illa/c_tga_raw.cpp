#include "StdAfx.h"
#include "c_tga_raw.h"

namespace Img {
	bool CodecTgaRawDecoder::OnProcess(Batch* batch) {
		switch(m_header->ColorDepth) {
			case 32:
			case 16:
			case 8:
				{
					size_t bytesPerScan = m_header->Size.Width * (m_header->ColorDepth >> 3);
					for(int currY = 0; currY < batch->NumRows; currY++) {
						m_reader->ReadFull(batch->Destination, bytesPerScan);
						batch->Destination += batch->SignedScanlineStride;
					}
				}
				break;
			case 24:
				for(int currY = 0; currY < batch->NumRows; currY++) {
					uint8_t* destStart = batch->Destination;
					uint32_t* destination = reinterpret_cast<uint32_t*>(destStart);
					for(int currX = 0; currX < m_header->Size.Width; currX++) {
						uint8_t b = m_fileStream.ReadByte();
						uint8_t g = m_fileStream.ReadByte();
						uint8_t r = m_fileStream.ReadByte();
						*(destination++) = Img::ToARGBDWORD(0xff, r, g, b);
					}
					destStart += batch->SignedScanlineStride;
					batch->Destination = destStart;
				}
				break;
			default:
				// Unsupported
				return true;
		}

		return false;
	}

	void CodecTgaRawDecoder::OnSource() {
		m_fileStream.SetFileReader(m_reader);
	}
}
