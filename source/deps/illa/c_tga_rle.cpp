#include "c_tga_rle.h"
#include <cstring>

namespace Img {
	struct TgaReadARGB {
		uint32_t operator()(IO::FileReaderByteStreamer& fs) const {
			uint8_t b = fs.ReadByte();
			uint8_t g = fs.ReadByte();
			uint8_t r = fs.ReadByte();
			uint8_t a = fs.ReadByte();
			return Img::ToARGBDWORD(a, r, g, b);
		}
		int PixelSize() const { return 4; }
	};

	struct TgaReadRGB {
		uint32_t operator()(IO::FileReaderByteStreamer& fs) const {
			uint8_t b = fs.ReadByte();
			uint8_t g = fs.ReadByte();
			uint8_t r = fs.ReadByte();
			return Img::ToARGBDWORD(0xff, r, g, b);
		}
		int PixelSize() const { return 3; }
	};

	struct TgaRead16 {
		uint16_t operator()(IO::FileReaderByteStreamer& fs) const {
			uint8_t x = fs.ReadByte();
			uint8_t y = fs.ReadByte();
			return (y << 8) + x;
		}
		int PixelSize() const { return 2; }
	};

	struct TgaRead8 {
		uint8_t operator()(IO::FileReaderByteStreamer& fs) const {
			return fs.ReadByte();
		}
		int PixelSize() const { return 1; }
	};

	using namespace Geom;
	bool CodecTgaRleDecoder::OnProcess(Batch* batch) {
		switch (m_header->ColorDepth) {
			case 32:
				return PerformOnProcess<uint32_t>(batch, TgaReadARGB());
			case 24:
				return PerformOnProcess<uint32_t>(batch, TgaReadRGB());
			case 16:
				return PerformOnProcess<uint16_t>(batch, TgaRead16());
			case 8:
				return PerformOnProcess<uint8_t>(batch, TgaRead8());
		}
		return true;
	}

	template <typename T_Pixel, class T_Read>
	bool CodecTgaRleDecoder::PerformOnProcess(Batch* batch, T_Read reader) {
		uint8_t* currentScanlineStartDestination = batch->Destination;
		int sourcePixelSize = reader.PixelSize();

		for (int currY = 0; currY < batch->NumRows; ++currY) {
			int currX = 0;
			batch->Destination = currentScanlineStartDestination;

			T_Pixel* alignedDestination = reinterpret_cast<T_Pixel*>(batch->Destination);

			while(currX < m_header->Size.Width) {
				if (m_numRlePixels == 0) {
					if (m_fileStream.BytesLeft() < (1 + sourcePixelSize)) {
						return true;
					}

					m_repField = m_fileStream.ReadByte();
					m_numRlePixels = (m_repField & 0x7F) + 1;
					m_isRleBlock = (m_repField & 0x80) != 0;

					if (m_isRleBlock) {
						auto* pixPtr = reinterpret_cast<T_Pixel*>(&m_pix);
						*pixPtr = reader(m_fileStream);
					}
				}

				int numPixelsToTransfer = std::min(m_header->Size.Width - currX, m_numRlePixels);
				m_numRlePixels -= numPixelsToTransfer;
				currX += numPixelsToTransfer;

				if (m_isRleBlock) {
					for (int i = 0; i < numPixelsToTransfer; ++i) {
						memcpy(alignedDestination++, &m_pix, sizeof(T_Pixel));
					}
				}
				else {
					if (m_fileStream.BytesLeft() < (numPixelsToTransfer * sourcePixelSize)) {
						return true;
					}

					for (int i = 0; i < numPixelsToTransfer; ++i) {
						*alignedDestination++ = reader(m_fileStream);
					}
				}
			}
			currentScanlineStartDestination += batch->SignedScanlineStride;
		}
		return false;
	}

	void CodecTgaRleDecoder::OnSource() {
		m_numRlePixels = 0;
		m_pix = 0xffffffff;

		m_fileStream.SetFileReader(m_reader);
	}
}
