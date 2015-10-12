#include "f_xyz.h"
#include "c_xyz.h"

namespace Img {
	AbstractCodec* FactoryXYZ::CreateCodec() {
		return new CodecXYZ();
	}

	FactoryXYZ::FactoryXYZ():ICodecFactory(u8"RPG Maker") {
		AddExtension(u8"XYZ");
	}
}
