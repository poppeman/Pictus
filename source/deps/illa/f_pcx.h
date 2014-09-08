#ifndef F_PCX_H
#define F_PCX_H

#include "c_factory.h"

namespace Img {
	class FactoryPCX:public ICodecFactory {
	public:
		AbstractCodec* CreateCodec() override;
		FactoryPCX();
	};
}

#endif
