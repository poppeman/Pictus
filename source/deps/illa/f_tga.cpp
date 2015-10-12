#include "f_tga.h"
#include "c_tga.h"

namespace Img {
	AbstractCodec* FactoryTGA::CreateCodec() {
		return new CodecTGA();
	}

	FactoryTGA::FactoryTGA():ICodecFactory(u8"Targa TGA") {
		AddExtension(u8"TGA");
	}
}
