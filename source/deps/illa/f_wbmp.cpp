#include "f_wbmp.h"
#include "c_wbmp.h"

namespace Img {
	AbstractCodec* FactoryWBMP::CreateCodec() {
		return new CodecWBMP();
	}

	FactoryWBMP::FactoryWBMP():ICodecFactory(TX("Wireless Bitmap")) {
		AddExtension(TX("WBMP"));
		AddExtension(TX("WBM"));
	}
}
