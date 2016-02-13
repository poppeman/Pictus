#ifndef VIEW_PAN_H
#define VIEW_PAN_H

#include "orz/geom.h"

namespace App {
	class ViewPan {
	public:
		void Reset();
		void ResizeViewport(const Geom::SizeInt& newViewportSize);
		const Geom::SizeInt& Viewport() const;

		void ResizeConstraints(const Geom::SizeInt& maxDims);
		const Geom::SizeInt& Constraints() const;

		void Pan(const Geom::SizeInt& distanceToPan);
		void SetCenter(const Geom::PointFloat& newCenter);

		const Geom::PointFloat Center() const;
		const Geom::PointInt TopLeft() const;
		const Geom::PointInt BottomRight() const;

	private:
		void Refresh();

		Geom::PointFloat m_center;
		Geom::SizeInt m_viewportSize;
		Geom::SizeInt m_constraints;

	};
}

#endif