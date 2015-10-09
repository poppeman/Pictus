#include "view_pan.h"
#include "orz/exception.h"
#include "orz/types.h"

namespace App {
	using namespace Geom;

	void ViewPan::Reset() {
		m_center = PointFloat(0, 0) + m_viewportSize * 0.5f;
	}

	void ViewPan::Pan(const Geom::SizeInt& distanceToPan) {
		m_center += distanceToPan;
		Refresh();
	}

	void ViewPan::SetCenter(const Geom::PointFloat& newCenter) {
		m_center = newCenter;
		Refresh();
	}

	void ViewPan::ResizeViewport(const Geom::SizeInt& newViewportSize) {
		m_viewportSize = newViewportSize;
		Refresh();
	}

	const Geom::SizeInt& ViewPan::Viewport() const {
		return m_viewportSize;
	}
	
	void ViewPan::ResizeConstraints(const Geom::SizeInt& maxDims) {
		SizeFloat scale = IsPositive(m_constraints)?maxDims.StaticCast<float>() / m_constraints.StaticCast<float>():SizeFloat(0, 0);
		m_center = m_center * scale;
		m_constraints = maxDims;
		Refresh();
	}

	const Geom::SizeInt& ViewPan::Constraints() const {
		return m_constraints;
	}

	const Geom::PointInt ViewPan::TopLeft() const {
		return Geom::Maximum(
			PointInt(0, 0), 
			(m_center - m_viewportSize * 0.5f).StaticCast<int>());
	}

	const Geom::PointFloat ViewPan::Center() const {
		return m_center.StaticCast<int>();
	}

	const Geom::PointInt ViewPan::BottomRight() const {
		return Geom::Minimum(
			PointInt(0, 0) + m_constraints,
			(m_center + m_viewportSize * 0.5f).StaticCast<int>());
	}

	void ViewPan::Refresh() {
		m_center = Geom::Constrain(
			PointFloat(0, 0) + m_viewportSize * 0.5f,
			m_center,
			PointFloat(0, 0) + m_constraints - m_viewportSize * 0.5f);

		if (m_center.AtMostInclusive(PointInt{ 0, 0 } +m_constraints) == false) {
			DO_THROW(Err::CriticalError, "Calculation error (Center).");
		}

		if (IsZeroOrPositive(TopLeft()) == false) {
			DO_THROW(Err::CriticalError, "Calculation error (TopLeft).");
		}

		if (BottomRight().AtMostInclusive(PointInt{ 0, 0 } + m_constraints) == false) {
			DO_THROW(Err::CriticalError, "Calculation error (BottomRight).");
		}
	}
}