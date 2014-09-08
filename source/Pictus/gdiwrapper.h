#ifndef GDIWRAPPER_H
#define GDIWRAPPER_H

#include "StdAfx.h"
#include "app_types.h"

namespace Win {
	class SharedDC {
	public:
		void BlitToDC(HDC destDC, const Geom::RectInt& destinationRect, const Geom::PointInt& sourceTopLeft);

		operator HDC& ();

		SharedDC();
		~SharedDC();

		void SelectObject(HGDIOBJ hObj);
		void UnselectObject();

	private:
		// No copies or assignment
		SharedDC(const SharedDC&);
		SharedDC& operator=(const SharedDC&);

	private:
		HDC m_hDC;
		HGDIOBJ m_hOldObj;
	};
}

#endif
