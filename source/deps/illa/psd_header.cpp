#include "StdAfx.h"
#include "psd_header.h"
#include "psd_shared.h"

namespace Img {
	namespace Internal {
		bool PsdHeader::ReadHeader( IO::FileReader::Ptr file ) {
			if (file->Read(Signature, 1, 4) != 4) {
				return false;
			}

			if (strncmp(Signature, "8BPS", 4) != 0) {
				return false;
			}

			if (ReadWordBE(file) != 1) {
				return false;
			}

			uint8_t reserved;
			for(int i = 0; i < 6; ++i)
			{
				if (file->Read(&reserved, 1, 1) != 1) {
					return false;
				}

				if (reserved != 0) {
					return false;
				}
			}

			Channels = ReadWordBE(file);
			Height = ReadDWordBE(file);
			Width = ReadDWordBE(file);

			BitsPerChannel = ReadWordBE(file);
			ColorMode = ColorModes(ReadWordBE(file));
			return true;
		}
	}
}
