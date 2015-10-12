#include "f_bmp.h"
#include "c_bmp.h"

namespace Img {
	AbstractCodec* FactoryBMP::CreateCodec() {
		return new CodecBMP();
	}

	FactoryBMP::FactoryBMP():ICodecFactory(u8"Windows Bitmap") {
		AddExtension(u8"BMP");
	}
}
