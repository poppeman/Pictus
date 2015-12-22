#include "surface_locked_area.h"

namespace Img {
	uint8_t* Surface::LockedArea::Buffer() {
		return m_buffer;
	}

	size_t Surface::LockedArea::Size() const {
		return m_rectLocked.Height() * m_stride;
	}

	void Surface::LockedArea::Unlock() {
		if (m_isLocked == false) DO_THROW(Err::CriticalError, "Attempted to unlock an already unlocked area.");
		m_surface->UnlockSurface(m_method);
		m_isLocked = false;
	}

	Surface::LockedArea::LockedArea( Surface* surface, uint8_t* buffer, const Geom::RectInt& rect, LockMethod method, size_t stride ):
		m_rectLocked(rect),
		m_buffer(buffer),
		m_stride(stride),
		m_method(method),
		m_surface(surface),
		m_isLocked(true)
	{
		if (m_surface == 0) DO_THROW(Err::InvalidParam, "Surface was null.");
		if (m_buffer == 0) DO_THROW(Err::InvalidParam, "Locked buffer was null.");
		if (m_stride == 0) DO_THROW(Err::InvalidParam, "Stride not set.");
	}

	Surface::LockedArea::~LockedArea() {
		if (m_isLocked)
			Unlock();
	}
}
