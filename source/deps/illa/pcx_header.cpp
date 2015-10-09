#include "pcx_header.h"
#include "color_io.h"

namespace Img {
	namespace Internal {
		bool PCXHeader::Load(IO::FileReader::Ptr file) {
			enum {
				Version_2_5				= 0,
				Version_2_8_PAL			= 2,
				Version_2_8_NOPAL		= 3,
				Version_3_0_PAL			= 5,
			};

			if (file->Size() < 130) {
				return false;
			}

			// Validate manufacturer
			uint8_t manufacturer;
			file->ReadFull(&manufacturer, 1);
			if (manufacturer != 10) {
				return false;
			}

			// Validate PCX version
			file->ReadFull(&Version, 1);
			if ((Version != Version_2_5) && (Version != Version_2_8_PAL) && (Version != Version_2_8_NOPAL) && (Version != Version_3_0_PAL)) {
				return false;
			}

			file->ReadFull(&IsCompressed, 1);
			file->ReadFull(&PlaneDepth, 1);

			signed short minx, miny, maxx, maxy;
			file->ReadFull(&minx, 2);
			file->ReadFull(&miny, 2);
			file->ReadFull(&maxx, 2);
			file->ReadFull(&maxy, 2);

			Size.Width	= maxx - minx + 1;
			Size.Height	= maxy - miny + 1;

			// Skip resolution shorts
			file->Seek(4, IO::SeekMethod::Current);

			// Wait a bit with palette
			FileInt palpos = file->Position();
			file->Seek(48, IO::SeekMethod::Current);

			file->Seek(1, IO::SeekMethod::Current);	// Skip reserved byte
			file->ReadFull(&ColorPlanes, 1);

			// Validate # of color planes
			if ((ColorPlanes != 1) && (ColorPlanes != 3) && (ColorPlanes != 4)) return false;

			file->ReadFull(&BytesPerLine, 2);
			file->ReadFull(&PaletteType, 2);

			// Go back to palette
			file->Seek(palpos, IO::SeekMethod::Begin);

			TotColorDepth = PlaneDepth * ColorPlanes;

			// Load palette (if applicable)
			if ((TotColorDepth == 8) && (Version == Version_3_0_PAL)) {
				file->Seek((long)(file->Size()-769), IO::SeekMethod::Begin);

				// Verify footer
				uint8_t ver;
				file->ReadFull(&ver, 1);
				if (ver == 12) {
					LoadPalette(Palette, *file, 0, 256, Img::LoadRGB);
				}
			}
			else if ((TotColorDepth <= 8) && ((Version == Version_2_5) || (Version == Version_2_8_PAL) || (Version == Version_3_0_PAL))) {
				LoadPalette(Palette, *file, 0, 16, Img::LoadRGB);
			}

			// Hax for monochrome images. Dunno if this is proper but I can't seem to find any proper palette info in.
			if (TotColorDepth == 1) {
				Palette.Color(0, Img::Color(0xff, 0, 0, 0));
				Palette.Color(1, Img::Color(0xff, 0xff, 0xff, 0xff));
			}

			if (8 * ColorPlanes * BytesPerLine < Size.Width * TotColorDepth) {
				return false;
			}

			file->Seek(128, IO::SeekMethod::Begin);
			return true;
		}
	}
}
