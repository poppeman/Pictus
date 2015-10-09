#ifndef SURFACEMGR_H
#define SURFACEMGR_H

#include "surface.h"
#include "types.h"
#include "surfacefactory.h"

namespace Img {
	struct FactoryNotSet:public Err::Exception {
		FactoryNotSet(const std::wstring&):Err::Exception(L"Surface Factory class not set. ") {}
	};

	void SurfaceFactory(ISurfaceFactory* pSurfaceFactory);

	Surface::Ptr CreateNewSurface();
	Surface::Ptr CreateNewSurface(const Geom::SizeInt& dimensions, Format fmt);
}

#endif
