#include "f_png.h"
#include "c_png.h"

namespace Img {
	AbstractCodec* FactoryPNG::CreateCodec() {
		return new CodecPNG();
	}

	FactoryPNG::FactoryPNG():ICodecFactory(u8"PNG (Portable Network Graphics)") {
		AddExtension(u8"PNG");
	}
}
