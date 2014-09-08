#ifndef F_BMP_H
#define F_BMP_H

#include "c_factory.h"

namespace Img {
	class FactoryBMP:public ICodecFactory {
	public:
		AbstractCodec* CreateCodec() override;
		FactoryBMP();
	};
}

#endif
