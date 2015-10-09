#include "f_tiff.h"
#include "c_tiff.h"

namespace Img {
	AbstractCodec* FactoryTIFF::CreateCodec() {
		return new CodecTIFF();
	}

	FactoryTIFF::FactoryTIFF():ICodecFactory(TX("TIFF")) {
		AddExtension(TX("TIF"));
		AddExtension(TX("TIFF"));
		AddExtension(TX("XIF"));
	}
}
