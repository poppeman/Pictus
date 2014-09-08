#include "StdAfx.h"
#include "f_xyz.h"
#include "c_xyz.h"

namespace Img {
	AbstractCodec* FactoryXYZ::CreateCodec() {
		return new CodecXYZ();
	}

	FactoryXYZ::FactoryXYZ():ICodecFactory(TX("RPG Maker")) {
		AddExtension(TX("XYZ"));
	}
}
