#include "f_pcx.h"
#include "c_pcx.h"

namespace Img {
	AbstractCodec* FactoryPCX::CreateCodec() {
		return new CodecPCX();
	}

	FactoryPCX::FactoryPCX():ICodecFactory(L"Z-soft PCX"){
		AddExtension(L"PCX");
	}
}
