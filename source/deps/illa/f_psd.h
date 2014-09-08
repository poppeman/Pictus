#ifndef F_PSD_H
#define F_PSD_H

#include "c_factory.h"

namespace Img {
	class FactoryPSD:public ICodecFactory {
	public:
		AbstractCodec* CreateCodec() override;
		FactoryPSD();
	};
}

#endif
