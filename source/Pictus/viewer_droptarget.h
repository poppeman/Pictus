#ifndef PICTUS_VIEWER_DROPTARGET_H
#define PICTUS_VIEWER_DROPTARGET_H

#include <wx/dnd.h>

namespace App
{
	class Viewer;

	class DropTarget:public wxFileDropTarget
	{
	public:
		virtual bool OnDropFiles(wxCoord x, wxCoord y, const wxArrayString &filenames) override;

		DropTarget(Viewer* v);

	private:
		Viewer* m_viewer;
	};
}

#endif
