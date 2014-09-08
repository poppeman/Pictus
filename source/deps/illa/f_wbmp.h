#ifndef F_WBMP_H
#define F_WBMP_H

#include "c_factory.h"

namespace Img {
	class FactoryWBMP:public ICodecFactory {
	public:
		AbstractCodec* CreateCodec() override;
		FactoryWBMP();
	};
}

#endif
