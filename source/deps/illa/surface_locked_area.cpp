#include "StdAfx.h"

#include "surface_locked_area.h"

namespace Img {
	uint8_t* Surface::LockedArea::Buffer() {
		return m_buffer;
	}

	void Surface::LockedArea::Unlock() {
		if (m_isLocked == false) DO_THROW(Err::CriticalError, TX("Attempted to unlock an already unlocked area."));
		m_surface->UnlockSurface(m_method);
		m_isLocked = false;
	}

	Surface::LockedArea::LockedArea( Surface* surface, uint8_t* buffer, const Geom::RectInt& rect, LockMethod method, size_t stride )
		:m_surface(surface),
		m_buffer(buffer),
		m_rectLocked(rect),
		m_method(method),
		m_stride(stride),
		m_isLocked(true)
	{
		if (m_surface == 0) DO_THROW(Err::InvalidParam, TX("Surface was null."));
		if (m_buffer == 0) DO_THROW(Err::InvalidParam, TX("Locked buffer was null."));
		if (m_stride == 0) DO_THROW(Err::InvalidParam, TX("Stride not set."));
	}

	Surface::LockedArea::~LockedArea() {
		if (m_isLocked)
			Unlock();
	}
}
