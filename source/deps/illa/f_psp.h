#ifndef F_PSP_H
#define F_PSP_H

#include "c_factory.h"

namespace Img {
	class FactoryPSP:public ICodecFactory {
	public:
		AbstractCodec* CreateCodec() override;
		FactoryPSP();
	};
}

#endif
