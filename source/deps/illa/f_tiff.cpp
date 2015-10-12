#include "f_tiff.h"
#include "c_tiff.h"

namespace Img {
	AbstractCodec* FactoryTIFF::CreateCodec() {
		return new CodecTIFF();
	}

	FactoryTIFF::FactoryTIFF():ICodecFactory(u8"TIFF") {
		AddExtension(u8"TIF");
		AddExtension(u8"TIFF");
		AddExtension(u8"XIF");
	}
}
