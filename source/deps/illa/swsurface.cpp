#include "StdAfx.h"

#include "swsurface.h"
#include "config.h"
#include "surface_lock_swmr.h"

namespace Img {
	using Geom::SizeInt;
	using Geom::RectInt;
	using Geom::PointInt;

	bool SurfaceSoftware::onCreateSurface() {
		m_pDIData.clear();
		try {
			m_pDIData.resize(static_cast<size_t>(Width()) * static_cast<size_t>(Height()) * static_cast<size_t>(PixelSize()));
			m_activeBufferPtr.store(m_pDIData.data());
		}
		catch(std::bad_alloc&) {
			return false;
		}
		return true;
	}

	uint8_t* SurfaceSoftware::onLockSurface(const RectInt& region, Img::LockMethod ) {
		auto curr = m_activeBufferPtr.load();
		if (curr == nullptr) {
			return nullptr;
		}

		return &curr[region.Left() * PixelSize() + region.Top() * onStride()];
	}

	void SurfaceSoftware::onUnlockSurface() {}

	void SurfaceSoftware::onPaletteUpdate() {
		ForceDirty();
	}

	size_t SurfaceSoftware::onStride() const {
		return static_cast<size_t>(GetSize().Width) * static_cast<size_t>(PixelSize());
	}

	SurfaceSoftware::SurfaceSoftware():m_activeBufferPtr(0) {
		SetLockStrategy(Img::LockStrategy::Ptr(new Img::LockStrategySingleWriterMultipleReaders));
	}

	uint8_t* SurfaceSoftware::DeviceIndependentData() {
		return m_activeBufferPtr.load();
	}

	void SurfaceSoftware::onDeallocate() {
		m_activeBufferPtr.store(nullptr);
		m_pDIData.clear();
	}
}
