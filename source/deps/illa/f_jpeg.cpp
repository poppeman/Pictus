#include "f_jpeg.h"
#include "c_jpeg.h"

namespace Img {
	AbstractCodec* FactoryJPEG::CreateCodec() {
		return new CodecJPEG();
	}

	FactoryJPEG::FactoryJPEG():ICodecFactory(L"JPEG (lossy)") {
		AddExtension(L"JPG");
		AddExtension(L"JPEG");
		AddExtension(L"JFIF");
		AddExtension(L"JPE");
	}
}
