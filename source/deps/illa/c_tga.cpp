
#include "c_tga.h"

#include "c_tga_raw.h"
#include "c_tga_rle.h"
#include "color_io.h"
#include "surface.h"
#include "surfacemgr.h"

namespace Img {
	using namespace Geom;

	bool CodecTGA::PerformLoadHeader(IO::FileReader::Ptr file, ImageInfo& info) {
		if (m_header.LoadHeader(file) == false)
			return false;

		if ((m_header.ImageType != TGAHeader::RawColormap) &&
			(m_header.ImageType != TGAHeader::RawTrueColor) &&
			(m_header.ImageType != TGAHeader::RawBW) &&
			(m_header.ImageType != TGAHeader::RLEColormap) &&
			(m_header.ImageType != TGAHeader::RLETrueColor) &&
			(m_header.ImageType != TGAHeader::RLEBW)) {
			return false;
		}

		if ((m_header.ColorDepth != 8) && (m_header.ColorDepth != 16) && (m_header.ColorDepth != 24) && (m_header.ColorDepth != 32)) {
			return false;
		}

		info.Dimensions		= m_header.Size;
		info.SurfaceFormat	= m_header.SurfaceFormat();

		// Move to correct position in the file
		file->Seek(m_header.IdSize, IO::SeekMethod::Current);

		if ((m_header.ImageType == TGAHeader::RLEColormap) || (m_header.ImageType == TGAHeader::RawColormap)) {
			if (m_header.ColorMapBits == 24) {
				LoadPalette(m_palette, *file, m_header.ColorMapStart, m_header.ColorMapLength, Img::LoadBGR);
			}

			if (m_header.ColorMapBits == 32) {
				LoadPalette(m_palette, *file, m_header.ColorMapStart, m_header.ColorMapLength, Img::LoadBGRA);
			}
		}
		else if (m_header.ImageType == TGAHeader::RawBW) {
			m_palette = Grayscale();
		}

		switch (m_header.ImageType) {
			case TGAHeader::RawBW:				// Uncompressed black and white (basically color mapped with a gray scale palette)
			case TGAHeader::RawColormap:		// Uncompressed color mapped
			case TGAHeader::RawTrueColor:		// Uncompressed true-color
				m_decoder = std::make_shared<CodecTgaRawDecoder>();
				break;
			case TGAHeader::RLEBW:				// Compressed gray scale
			case TGAHeader::RLEColormap:       // Compressed color mapped
			case TGAHeader::RLETrueColor:		// Compressed true color
				m_decoder = std::make_shared<CodecTgaRleDecoder>();
				break;
			default:
				return false;
		}

		m_decoder->SetSource(file, &m_header);

		return true;
	}

	AbstractCodec::AllocationStatus CodecTGA::PerformAllocate() {
		GetSurface()->CreateSurface(m_header.Size, m_header.SurfaceFormat());
		GetSurface()->ClearSurface(Img::Color(0xff, 0xff, 0xff, 0xff));
		if (m_header.SurfaceFormat() == Format::Index8) {
			GetSurface()->SetPalette(m_palette);
		}

		m_decoder->SetDestination(GetSurface());

		return AllocationStatus::Ok;
	}

	AbstractCodec::LoadStatus CodecTGA::PerformLoadImageData(IO::FileReader::Ptr file) {
		while (DoTerminate() == false) {
			if (m_decoder->Process() == true) {
				return LoadStatus::Finished;
			}
		}

		return LoadStatus::Aborted;
	}
}
