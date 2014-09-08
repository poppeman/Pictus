#ifndef F_JPEG_H
#define F_JPEG_H

#include "c_factory.h"

namespace Img {
	class FactoryJPEG:public ICodecFactory {
	public:
		AbstractCodec* CreateCodec() override;
		FactoryJPEG();
	};
}

#endif
