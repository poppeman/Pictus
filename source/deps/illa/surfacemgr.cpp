#include "surfacemgr.h"
#include "surface.h"
#include "surfacefactory.h"

namespace Img {

	FactoryNotSet::FactoryNotSet(const std::string&):
		Err::Exception("Surface Factory class not set. ")
	{}



	ISurfaceFactory* g_pSurfaceFactory = 0;

	void SurfaceFactory(ISurfaceFactory* pSurfaceFactory) {
		g_pSurfaceFactory = pSurfaceFactory;
	}

	Surface::Ptr CreateNewSurface() {
		// Sanity check
		if (g_pSurfaceFactory == nullptr) {
			DO_THROW(FactoryNotSet, "Factory not set lul");
		}

		return g_pSurfaceFactory->CreateSurface();
	}

	Surface::Ptr CreateNewSurface( const Geom::SizeInt& dimensions, Format fmt ) {
		Surface::Ptr surface = CreateNewSurface();
		surface->CreateSurface(dimensions, fmt);
		return surface;
	}
}
