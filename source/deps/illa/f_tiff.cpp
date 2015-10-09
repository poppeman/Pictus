#include "f_tiff.h"
#include "c_tiff.h"

namespace Img {
	AbstractCodec* FactoryTIFF::CreateCodec() {
		return new CodecTIFF();
	}

	FactoryTIFF::FactoryTIFF():ICodecFactory(L"TIFF") {
		AddExtension(L"TIF");
		AddExtension(L"TIFF");
		AddExtension(L"XIF");
	}
}
