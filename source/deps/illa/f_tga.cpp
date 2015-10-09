#include "f_tga.h"
#include "c_tga.h"

namespace Img {
	AbstractCodec* FactoryTGA::CreateCodec() {
		return new CodecTGA();
	}

	FactoryTGA::FactoryTGA():ICodecFactory(TX("Targa TGA")) {
		AddExtension(TX("TGA"));
	}
}
