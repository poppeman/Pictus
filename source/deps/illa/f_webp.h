#ifndef F_WEBP_H
#define F_WEBP_H

#include "c_factory.h"

namespace Img {
	class FactoryWebp:public ICodecFactory {
	public:
		AbstractCodec* CreateCodec() override;
		FactoryWebp();
	};
}

#endif
