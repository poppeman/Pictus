#include "gif_imagedescriptor.h"
#include "gif_io.h"

namespace Img {
	bool ImageDescriptor::ReadGraphicControlExtBlock(IO::FileReader::Ptr file) {
		uint8_t size;
		file->ReadFull(&size, 1);
		if (size != 4) {
			return false;
		}

		uint8_t packed;
		file->ReadFull(&packed, 1);
		short sDelay;
		file->ReadFull(&sDelay, 2);

		Delay = sDelay * 9;

		if (Delay < 10) Delay = DefaultDelay;

		file->ReadFull(&ColorKey, 1);
		IsTransparent = ((packed & Mask_GC_Transparent) != 0);
		int dm = (DisposeMethod)((packed & Mask_GC_Dispose) >> 2);
		if (dm >= 0 && dm <= 3) {
			Dispose = (DisposeMethod)dm;
		}
		return true;
	}

	bool ImageDescriptor::ReadDescriptor(IO::FileReader::Ptr file) {
		uint16_t Left, Top;
		file->ReadFull(&Left, 2);
		file->ReadFull(&Top, 2);
		TopLeft.X = Left;
		TopLeft.Y = Top;

		ReadSizeWord(&ImageSize, file);

		uint8_t packed;
		file->ReadFull(&packed, 1);

		// Inspect flags.
		LCTExist	= ((packed & Mask_ImageDesc_LCTExist)!=0);
		Interlace	= ((packed & Mask_ImageDesc_Interlace)!=0);
		LCTSorted	= ((packed & Mask_ImageDesc_Sort)!=0);
		LCTSize		= static_cast<short>(2 << ((packed & Mask_ImageDesc_LCTSize)));

		// Disable LCT if size=0
		if (LCTSize == 0) {
			LCTExist = false;
		}

		// Load Local Color Table
		if (LCTExist) {
			ReadPalette(LCTEntries, LCTSize, file);
		}

		FileOffset = file->Position();		// Remember image data position
		file->Seek(1, IO::SeekMethod::Current);	// Skip LZW minimum code size. Data blocks should follow, starting with block size.

		return true;
	}
}
