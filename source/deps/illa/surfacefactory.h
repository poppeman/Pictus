#ifndef SURFACEFACTORY_H
#define SURFACEFACTORY_H

#include "surface.h"
#include "types.h"

namespace Img {
	class Surface;

	class ISurfaceFactory {
	public:
		virtual Surface::Ptr CreateSurface()=0;

		ISurfaceFactory() {}
		virtual ~ISurfaceFactory() {}
	};
}

#endif
