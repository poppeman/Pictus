#include "f_pcx.h"
#include "c_pcx.h"

namespace Img {
	AbstractCodec* FactoryPCX::CreateCodec() {
		return new CodecPCX();
	}

	FactoryPCX::FactoryPCX():ICodecFactory(TX("Z-soft PCX")){
		AddExtension(TX("PCX"));
	}
}
