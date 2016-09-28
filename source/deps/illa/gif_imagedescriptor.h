#ifndef GIF_IMAGEDESCRIPTOR_H
#define GIF_IMAGEDESCRIPTOR_H

#include "orz/file_reader.h"
#include "surface.h"
#include "types.h"

namespace Img {
	class ImageDescriptor {
	public:
		enum {
			DefaultDelay = 80,

			// Image descriptor packed masks.
			Mask_ImageDesc_LCTExist	= 0x80,
			Mask_ImageDesc_Interlace= 0x40,
			Mask_ImageDesc_Sort		= 0x20,
			Mask_ImageDesc_Reserved	= 0x18,
			Mask_ImageDesc_LCTSize	= 0x07,

			// Graphics control extension block masks
			Mask_GC_Dispose			= 0x1c,
			Mask_GC_UserInput		= 0x02,
			Mask_GC_Transparent		= 0x01,

		};

		enum DisposeMethod {
			DisposeUnknown=0,
			DisposeNoDispose=1,
			DisposeBackground=2,
			DisposeToPrevious=3
		};

		Geom::PointInt TopLeft;
		Geom::SizeInt ImageSize;

		bool Interlace = false;

		// Local Color Table
		bool LCTExist = false;
		bool LCTSorted = false;
		short LCTSize = 0;
		Img::Palette LCTEntries;

		// Surface for this frame.
		Img::Surface::Ptr Surface;

		// Location of image data.
		FileInt FileOffset = 0;

		// Additional settings from extended block
		int Delay = DefaultDelay;
		uint8_t ColorKey = 0;
		bool IsTransparent = false;
		DisposeMethod Dispose = DisposeUnknown;

		bool ReadGraphicControlExtBlock(IO::FileReader::Ptr file);
		bool ReadDescriptor(IO::FileReader::Ptr file);

		typedef std::shared_ptr<ImageDescriptor> Ptr;
	};

	typedef std::vector<ImageDescriptor::Ptr> DescriptorVector;
	typedef std::shared_ptr<DescriptorVector> DescriptorVectorPtr;
}

#endif
