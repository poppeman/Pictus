#ifndef VIEWER_CONTEXTMENU_H
#define VIEWER_CONTEXTMENU_H

#include "actionmap.h"
#include "app_types.h"

#include <wx/menu.h>

namespace App
{
	class Viewer;

	class ViewerContextMenu:public wxMenu
	{
	public:
		void Zoomed(bool fullSize);
		void FitImage();

		void Construct(Viewer*v);

		ViewerContextMenu();
		~ViewerContextMenu();

	private:
		void OnMenu(wxCommandEvent &evt);

		wxMenu* DoAddSubMenu(StringID label);

		wxMenuItem *m_zoomFitImage = nullptr, *m_zoomFullSize = nullptr;

		typedef ActionMapNoParam<int> MenuActionMap;
		MenuActionMap m_menuMap;

		DECLARE_EVENT_TABLE()
	};
}

#endif
