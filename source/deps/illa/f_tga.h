#ifndef F_TGA_H
#define F_TGA_H

#include "c_factory.h"

namespace Img {
	class FactoryTGA:public ICodecFactory {
	public:
		AbstractCodec* CreateCodec() override;
		FactoryTGA();
	};
}

#endif
