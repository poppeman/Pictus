#include "f_wbmp.h"
#include "c_wbmp.h"

namespace Img {
	AbstractCodec* FactoryWBMP::CreateCodec() {
		return new CodecWBMP();
	}

	FactoryWBMP::FactoryWBMP():ICodecFactory(u8"Wireless Bitmap") {
		AddExtension(u8"WBMP");
		AddExtension(u8"WBM");
	}
}
