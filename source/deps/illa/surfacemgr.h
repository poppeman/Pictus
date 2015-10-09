#ifndef SURFACEMGR_H
#define SURFACEMGR_H

#include "surface.h"
#include "types.h"
#include "surfacefactory.h"

namespace Img {
	struct FactoryNotSet:public Err::Exception {
		FactoryNotSet(const std::string&);
	};

	void SurfaceFactory(ISurfaceFactory* pSurfaceFactory);

	Surface::Ptr CreateNewSurface();
	Surface::Ptr CreateNewSurface(const Geom::SizeInt& dimensions, Format fmt);
}

#endif
