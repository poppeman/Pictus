#ifndef F_TIFF_H
#define F_TIFF_H

#include "c_factory.h"

namespace Img {
	class FactoryTIFF:public ICodecFactory {
	public:
		AbstractCodec* CreateCodec() override;
		FactoryTIFF();
	};
}

#endif
