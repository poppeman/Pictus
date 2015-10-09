#include "f_gif.h"
#include "c_gif.h"

namespace Img {
	AbstractCodec* FactoryGIF::CreateCodec() {
		return new CodecGIF();
	}

	FactoryGIF::FactoryGIF():ICodecFactory(L"Compuserve GIF") {
		AddExtension(L"GIF");
	}
}
