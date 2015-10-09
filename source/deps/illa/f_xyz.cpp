#include "f_xyz.h"
#include "c_xyz.h"

namespace Img {
	AbstractCodec* FactoryXYZ::CreateCodec() {
		return new CodecXYZ();
	}

	FactoryXYZ::FactoryXYZ():ICodecFactory(L"RPG Maker") {
		AddExtension(L"XYZ");
	}
}
