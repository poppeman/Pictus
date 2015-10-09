#include "f_psp.h"
#include "c_psp.h"

namespace Img {
	AbstractCodec* FactoryPSP::CreateCodec() {
		return new CodecPSP();
	}

	FactoryPSP::FactoryPSP():ICodecFactory(L"Corel Paint Shop Pro") {
		AddExtension(L"PSP");
		AddExtension(L"PSPIMAGE");
	}
}
