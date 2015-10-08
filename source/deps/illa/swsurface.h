#ifndef SWSURFACE_H
#define SWSURFACE_H

#include "surface.h"
#include "surfacefactory.h"
#include <atomic>

namespace Img {
	class SurfaceSoftware:public Img::Surface {
	public:
		uint8_t* DeviceIndependentData();

		SurfaceSoftware();

		typedef std::shared_ptr<SurfaceSoftware> Ptr;

	private:
		void onDeallocate();
		bool onCreateSurface();

		unsigned char* onLockSurface(const Geom::RectInt& region, Img::LockMethod method = Img::LockReadWrite);
		void onUnlockSurface();
		void onPaletteUpdate();
		size_t onStride() const;

		std::vector<uint8_t> m_pDIData;
		std::atomic<uint8_t*> m_activeBufferPtr;
	};

	class FactorySurfaceSoftware:public Img::ISurfaceFactory {
	public:
		Img::Surface::Ptr CreateSurface() { return Surface::Ptr(new SurfaceSoftware); }
	};
}

#endif
