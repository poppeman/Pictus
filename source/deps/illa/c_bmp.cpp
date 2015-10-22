#include "orz/orz_math.h"
#include "c_bmp.h"
#include "c_bmp_raw_4.h"
#include "c_bmp_raw_8.h"
#include "c_bmp_raw_16.h"
#include "c_bmp_raw_24.h"
#include "c_bmp_rle_4.h"
#include "c_bmp_rle_8.h"
#include "config.h"
#include "surface_locked_area.h"

namespace Img {
	using namespace Geom;
	using namespace Internal;

	bool CodecBMP::PerformLoadHeader(IO::FileReader::Ptr file, ImageInfo& info) {
		m_bfh.Load(file);
		m_header.Load(file);

		// Pick surface format
		info.SurfaceFormat = Img::Format::Undefined;

		if ((m_header.ColorDepth == 1) || (m_header.ColorDepth == 4) || (m_header.ColorDepth == 8)) {
			info.SurfaceFormat = Img::Format::Index8;
		}
		else if (m_header.ColorDepth == 16) {
			if (Math::Bit::CountSet(m_header.GreenMask) == 6) {
				info.SurfaceFormat = Img::Format::RGB565;
			}
			else {
				info.SurfaceFormat = Img::Format::XRGB1555;
			}
		}
		else if ((m_header.ColorDepth == 24) || (m_header.ColorDepth == 32)) {
			info.SurfaceFormat = Img::Format::XRGB8888;
		}
		else {
			DO_THROW(Err::InvalidHeader, "CodecBMP: BMP unsupported color depth");
		}

		file->Seek(m_bfh.offset, IO::SeekMethod::Begin);
		m_fileStream.SetFileReader(file);

		info.Dimensions = m_header.Size;

		if (m_header.Compression == BMPHeader::CompressRGB || m_header.Compression == BMPHeader::CompressBitfields)
		{
			if (m_header.ColorDepth == 1 || m_header.ColorDepth == 4)
			{
				m_dec = std::make_shared<Internal::BmpRawDecoder4>(GetSurface(), m_header, file);
			}
			if (m_header.ColorDepth == 8)
			{
				m_dec = std::make_shared<Internal::BmpRawDecoder8>(GetSurface(), m_header, file);
			}
			if (m_header.ColorDepth == 16)
			{
				m_dec = std::make_shared<Internal::BmpRawDecoder16>(GetSurface(), m_header, file);
			}
			if (m_header.ColorDepth == 24 || m_header.ColorDepth == 32)
			{
				m_dec = std::make_shared<Internal::BmpRawDecoder24>(GetSurface(), m_header, file);
			}
		}
		else if (m_header.Compression == BMPHeader::CompressRLE4)
		{
			if (m_header.ColorDepth == 4)
			{
				m_dec = std::make_shared<Internal::BmpRleDecoder4>(GetSurface(), m_header, file);
			}
		}
		else if (m_header.Compression == BMPHeader::CompressRLE8)
		{
			if (m_header.ColorDepth == 8)
			{
				m_dec = std::make_shared<Internal::BmpRleDecoder8>(GetSurface(), m_header, file);
			}
		}
		
		if(m_dec == nullptr)
		{
			DO_THROW(Err::CodecError, u8"BMP data format not recognized: " + ToAString(m_header.Compression));
		}

		return true;
	}

	AbstractCodec::AllocationStatus CodecBMP::PerformAllocate() {
		GetSurface()->CreateSurface(GetSize(), GetFormat());
		if (GetFormat() == Format::Index8) {
			GetSurface()->SetPalette(m_header.Palette);
		}
		return AllocationStatus::Ok;
	}

	AbstractCodec::LoadStatus CodecBMP::PerformLoadImageData(IO::FileReader::Ptr file) {
		while (DoTerminate() == false) {
			if (m_dec->Process())
			{
				return LoadStatus::Finished;
			}
		}

		return LoadStatus::Aborted;
	}

	CodecBMP::CodecBMP() {}
}
