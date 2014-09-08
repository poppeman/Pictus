#ifndef F_PNG_H
#define F_PNG_H

#include "c_factory.h"

namespace Img {
	class FactoryPNG:public ICodecFactory {
	public:
		AbstractCodec* CreateCodec() override;
		FactoryPNG();
	};
}

#endif
