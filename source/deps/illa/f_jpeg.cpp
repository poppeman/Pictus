#include "f_jpeg.h"
#include "c_jpeg.h"

namespace Img {
	AbstractCodec* FactoryJPEG::CreateCodec() {
		return new CodecJPEG();
	}

	FactoryJPEG::FactoryJPEG():ICodecFactory(TX("JPEG (lossy)")) {
		AddExtension(TX("JPG"));
		AddExtension(TX("JPEG"));
		AddExtension(TX("JFIF"));
		AddExtension(TX("JPE"));
	}
}
