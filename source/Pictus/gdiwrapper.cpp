#include "StdAfx.h"
#include "gdiwrapper.h"

namespace Win {
	void SharedDC::BlitToDC(HDC destDC, const Geom::RectInt& destinationRect, const Geom::PointInt& sourceTopLeft) {
		COND_STRICT(BitBlt(destDC, destinationRect.Left(), destinationRect.Top(), destinationRect.Width(), destinationRect.Height(), *this, sourceTopLeft.X, sourceTopLeft.Y, SRCCOPY), Err::CriticalError, TX("SharedDC::BlitToDC, BitBlt failed: ") + ToWString(GetLastError()));
	}

	 SharedDC::SharedDC():m_hDC(0), m_hOldObj(0) {
		HDC hMonitorDC = GetDC(0);
		m_hDC = CreateCompatibleDC(hMonitorDC);
		ReleaseDC(0, hMonitorDC);
	}

	 SharedDC::~SharedDC() {
		UnselectObject();
		DeleteDC(m_hDC);
	}

	void SharedDC::SelectObject(HGDIOBJ hObj) {
		m_hOldObj = ::SelectObject(m_hDC, hObj);
	}
	void SharedDC::UnselectObject() {
		if (m_hOldObj)
			::SelectObject(m_hDC, m_hOldObj);

		m_hOldObj = 0;
	}

	SharedDC::operator HDC&() {
		return m_hDC;
	}
}
