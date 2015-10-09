#include "f_psd.h"
#include "c_psd.h"

namespace Img {
	AbstractCodec* FactoryPSD::CreateCodec() {
		return new CodecPSD();
	}

	FactoryPSD::FactoryPSD():ICodecFactory(TX("Adobe Photoshop")) {
		AddExtension(TX("PSD"));
	}
}