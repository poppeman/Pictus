#include "f_psp.h"
#include "c_psp.h"

namespace Img {
	AbstractCodec* FactoryPSP::CreateCodec() {
		return new CodecPSP();
	}

	FactoryPSP::FactoryPSP():ICodecFactory(u8"Corel Paint Shop Pro") {
		AddExtension(u8"PSP");
		AddExtension(u8"PSPIMAGE");
	}
}
