#include "c_wbmp.h"
#include "orz/bitconvert.h"
#include "surface_locked_area.h"

namespace Img {
	bool CodecWBMP::PerformLoadHeader( IO::FileReader::Ptr file, ImageInfo& info ) {
		// The standard only covers type 0 images, so no need to be any more advanced here
		if (ReadUIntVar(file) != 0) return false;

		uint8_t sign;
		file->ReadFull(&sign, 1);
		if (sign != 0) return false;

		info.Dimensions.Width = ReadUIntVar(file);
		info.Dimensions.Height = ReadUIntVar(file);
		info.SurfaceFormat = Img::Format::Index8;

		m_currScan = 0;

		return true;
	}

	AbstractCodec::AllocationStatus CodecWBMP::PerformAllocate() {
		GetSurface()->CreateSurface(GetSize(), GetFormat());
		GetSurface()->ClearSurface(Img::Color(0xff, 0xff, 0xff, 0xff));

		GetSurface()->SetPalette(Img::Grayscale(0, 1));
		return AllocationStatus::Ok;
	}

	AbstractCodec::LoadStatus CodecWBMP::PerformLoadImageData( IO::FileReader::Ptr file ) {
		using namespace Geom;

		int width = (GetSize().Width);
		int scanBytes = (width + 7) / 8;
		auto currScan=std::make_unique<uint8_t[]>(scanBytes);

		while (DoTerminate() == false) {
			if (m_currScan == GetSize().Height)
				return LoadStatus::Finished;

			auto area = GetSurface()->LockSurface(RectInt(PointInt(0, m_currScan), SizeInt(width, 1)));

			file->ReadFull(currScan.get(), scanBytes);
			Util::BitsToBytes(area->Buffer(), currScan.get(), width);
			m_currScan++;
		}
		return LoadStatus::Aborted;
	}

	uint32_t CodecWBMP::ReadUIntVar( IO::FileReader::Ptr file ) {
		uint8_t b;
		uint32_t currentValue = 0;

		do {
			file->ReadFull(&b, 1);
			currentValue = (currentValue << 7) + (b & 0x7F);
		} while((b & 0x80) != 0);

		return currentValue;
	}
}
