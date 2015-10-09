#include "f_bmp.h"
#include "c_bmp.h"

namespace Img {
	AbstractCodec* FactoryBMP::CreateCodec() {
		return new CodecBMP();
	}

	FactoryBMP::FactoryBMP():ICodecFactory(TX("Windows Bitmap")) {
		AddExtension(TX("BMP"));
	}
}
