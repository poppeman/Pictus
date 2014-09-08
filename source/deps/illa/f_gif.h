#ifndef F_GIF_H
#define F_GIF_H

#include "c_factory.h"

namespace Img {
	class FactoryGIF:public ICodecFactory {
	public:
		AbstractCodec* CreateCodec() override;
		FactoryGIF();
	};
}

#endif