#include "f_pcx.h"
#include "c_pcx.h"

namespace Img {
	AbstractCodec* FactoryPCX::CreateCodec() {
		return new CodecPCX();
	}

	FactoryPCX::FactoryPCX():ICodecFactory(u8"Z-soft PCX"){
		AddExtension(u8"PCX");
	}
}
