#include "viewer_droptarget.h"
#include "viewer.h"

namespace App
{
	bool DropTarget::OnDropFiles(wxCoord x, wxCoord y, const wxArrayString &filenames)
	{
		//SetImageLocation(WStringToUTF8(reinterpret_cast<wchar_t*>(pcds->lpData)));
		m_viewer->SetImageLocation(WStringToUTF8(filenames[0].c_str().AsWChar()));
		return true;
	}

	DropTarget::DropTarget(Viewer *v):
		m_viewer(v)
	{
	}
}
