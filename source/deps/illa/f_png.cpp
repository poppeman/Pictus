#include "f_png.h"
#include "c_png.h"

namespace Img {
	AbstractCodec* FactoryPNG::CreateCodec() {
		return new CodecPNG();
	}

	FactoryPNG::FactoryPNG():ICodecFactory(L"PNG (Portable Network Graphics)") {
		AddExtension(L"PNG");
	}
}
