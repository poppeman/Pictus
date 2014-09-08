#ifndef F_XYZ_H
#define F_XYZ_H

#include "c_factory.h"

namespace Img {
	class FactoryXYZ:public ICodecFactory {
	public:
		AbstractCodec* CreateCodec() override;
		FactoryXYZ();
	};
}

#endif
