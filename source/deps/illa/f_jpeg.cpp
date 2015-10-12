#include "f_jpeg.h"
#include "c_jpeg.h"

namespace Img {
	AbstractCodec* FactoryJPEG::CreateCodec() {
		return new CodecJPEG();
	}

	FactoryJPEG::FactoryJPEG():ICodecFactory(u8"JPEG (lossy)") {
		AddExtension(u8"JPG");
		AddExtension(u8"JPEG");
		AddExtension(u8"JFIF");
		AddExtension(u8"JPE");
	}
}
