#include "f_webp.h"
#include "c_webp.h"

namespace Img {
	AbstractCodec* FactoryWebp::CreateCodec() {
		return new CodecWebp();
	}

	FactoryWebp::FactoryWebp():ICodecFactory(u8"WebP") {
		AddExtension(u8"WEBP");
	}
}
