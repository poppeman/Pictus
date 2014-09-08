#ifndef VIEWER_CONTEXTMENU_H
#define VIEWER_CONTEXTMENU_H

#include "menu.h"
#include "actionmap.h"

namespace Win {
	class BaseWindow;
}

namespace App {
	class Viewer;

	class ViewerContextMenu {
	public:
		void Display(Geom::PointInt pos);

		void Zoomed(bool fullSize);
		void FitImage();

		void Construct(Viewer* v);

		ViewerContextMenu();

	private:
		Win::Menu m_menu;

		Viewer* m_viewer;

		typedef ActionMapNoParam<DWORD> MenuActionMap;
		MenuActionMap m_menuMap;
	};
}

#endif
