#ifndef ILLA_SURFACE_LOCKED_AREA_H
#define ILLA_SURFACE_LOCKED_AREA_H

#include "surface.h"

namespace Img {
	class Surface::LockedArea {
	public:
		size_t Stride() const;
		uint8_t* Buffer();

		void Unlock();

		LockedArea(Surface* surface, uint8_t* buffer, const Geom::RectInt& rect, LockMethod isReadOnly, size_t stride);
		~LockedArea();

		typedef std::shared_ptr<LockedArea> Ptr;

	private:
		Geom::RectInt m_rectLocked;
		uint8_t* m_buffer;
		size_t m_stride;
		LockMethod m_method;
		Surface* m_surface;
		bool m_isLocked;
	};
}

#endif
