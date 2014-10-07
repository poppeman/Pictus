#include "StdAfx.h"
#include "f_psp.h"
#include "c_psp.h"

namespace Img {
	AbstractCodec* FactoryPSP::CreateCodec() {
		return new CodecPSP();
	}

	FactoryPSP::FactoryPSP():ICodecFactory(TX("Corel Paint Shop Pro")) {
		AddExtension(TX("PSP"));
		AddExtension(TX("PSPIMAGE"));
	}
}
