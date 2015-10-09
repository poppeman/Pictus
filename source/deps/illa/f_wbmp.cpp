#include "f_wbmp.h"
#include "c_wbmp.h"

namespace Img {
	AbstractCodec* FactoryWBMP::CreateCodec() {
		return new CodecWBMP();
	}

	FactoryWBMP::FactoryWBMP():ICodecFactory(L"Wireless Bitmap") {
		AddExtension(L"WBMP");
		AddExtension(L"WBM");
	}
}
