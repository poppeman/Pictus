#include "viewer_contextmenu.h"
#include "viewer.h"
//#include <VersionHelpers.h>

namespace App {
	using namespace Win;

	BEGIN_EVENT_TABLE(ViewerContextMenu, wxMenu)
		EVT_MENU(wxID_ANY, ViewerContextMenu::OnMenu)
	END_EVENT_TABLE()

	int DoAddMenuItem(wxMenu* menu, StringID label)
	{
		menu->Append(label, Win::GetStringWx(label));
		return label;
	}


	void ViewerContextMenu::Construct(Viewer* v)
	{
		m_menuMap.AddAction(DoAddMenuItem(this, SIDMenuOpen), [=]() { v->OpenFolder(); });

		auto pZoom = DoAddSubMenu(SIDMenuZoom);
		m_zoomFitImage = pZoom->AppendCheckItem(SIDActionZoomFitImage, Win::GetStringWx(SIDActionZoomFitImage));
		m_menuMap.AddAction(m_zoomFitImage->GetId(), [=]() { v->ZoomMode(ZoomFitImage); });
		m_zoomFullSize = pZoom->AppendCheckItem(SIDActionZoomFullSize, Win::GetStringWx(SIDActionZoomFullSize));
		m_menuMap.AddAction(m_zoomFullSize->GetId(), [=]() { v->ZoomMode(ZoomFullSize); });

		pZoom->AppendSeparator();
		m_menuMap.AddAction(DoAddMenuItem(pZoom, SIDActionZoomIn), [=]() { v->ZoomIn(); });
		m_menuMap.AddAction(DoAddMenuItem(pZoom, SIDActionZoomOut), [=]() { v->ZoomOut(); });

		auto pRot = DoAddSubMenu(SIDMenuOrientation);
		m_menuMap.AddAction(DoAddMenuItem(pRot, SIDMenuOrientationNoRotation), [=]() { v->Rotate(Filter::RotationAngle::RotateDefault); });
		m_menuMap.AddAction(DoAddMenuItem(pRot, SIDMenuOrientationMirror), [=]() { v->Rotate(Filter::RotationAngle::FlipX); });
		m_menuMap.AddAction(DoAddMenuItem(pRot, SIDMenuOrientationFlip), [=]() { v->Rotate(Filter::RotationAngle::FlipY); });
		m_menuMap.AddAction(DoAddMenuItem(pRot, SIDMenuOrientationRotate90), [=]() { v->Rotate(Filter::RotationAngle::Rotate90); });
		m_menuMap.AddAction(DoAddMenuItem(pRot, SIDMenuOrientationRotate90Flip), [=]() { v->Rotate(Filter::RotationAngle::Rotate90FlipY); });
		m_menuMap.AddAction(DoAddMenuItem(pRot, SIDMenuOrientationRotate180), [=]() { v->Rotate(Filter::RotationAngle::Rotate180); });
		m_menuMap.AddAction(DoAddMenuItem(pRot, SIDMenuOrientationRotate270), [=]() { v->Rotate(Filter::RotationAngle::Rotate270); });
		m_menuMap.AddAction(DoAddMenuItem(pRot, SIDMenuOrientationRotate270Flip), [=]() { v->Rotate(Filter::RotationAngle::Rotate270FlipY); });

		auto pSort = DoAddSubMenu(SIDMenuSortBy);
		m_menuMap.AddAction(DoAddMenuItem(pSort, SIDMenuSortByFilename), [=]() { v->Sort(Img::Cacher::SortMethod::SortFilename); });
		m_menuMap.AddAction(DoAddMenuItem(pSort, SIDMenuSortByDateModified), [=]() { v->Sort(Img::Cacher::SortMethod::SortDateModified); });
		m_menuMap.AddAction(DoAddMenuItem(pSort, SIDMenuSortByDateCreated), [=]() { v->Sort(Img::Cacher::SortMethod::SortDateCreated); });

		// TODO: Reimplement wallpaper support (based on platform capabilities)
		/*auto pWall = DoAddSubMenu(SIDMenuSetWallpaper));

		if (IsWindows7OrGreater())
		{
			m_menuMap.AddAction(pWall->AddItem(SIDMenuSetWallpaperCropFill), [=] { v->ApplyWallpaper(Win::Wallpaper::Mode::FillCrop); });
			m_menuMap.AddAction(pWall->AddItem(SIDMenuSetWallpaperPadFill), [=] { v->ApplyWallpaper(Win::Wallpaper::Mode::FillPad); });
		}

		if (IsWindows8OrGreater())
		{
			m_menuMap.AddAction(pWall->AddItem(SIDMenuSetWallpaperSpan), [=] { v->ApplyWallpaper(Win::Wallpaper::Mode::Span); });
		}

		m_menuMap.AddAction(pWall->AddItem(SIDMenuSetWallpaperStretch), [=]() { v->ApplyWallpaper(Win::Wallpaper::Mode::Stretch); });
		m_menuMap.AddAction(pWall->AddItem(SIDMenuSetWallpaperCenter), [=]() { v->ApplyWallpaper(Win::Wallpaper::Mode::Center); });
		m_menuMap.AddAction(pWall->AddItem(SIDMenuSetWallpaperTile), [=]() { v->ApplyWallpaper(Win::Wallpaper::Mode::Tile); });*/

		m_menuMap.AddAction(DoAddMenuItem(this, SIDMenuShowExplorer), [=]() { v->OpenDirectoryInExplorer(); });
		m_menuMap.AddAction(DoAddMenuItem(this, SIDMenuAdjust), [=]() { v->ShowAdjust(); });
		m_menuMap.AddAction(DoAddMenuItem(this, SIDMenuRename), [=]() { v->RenameCurrent(); });
		m_menuMap.AddAction(DoAddMenuItem(this, SIDMenuSettings), [=]() { v->ShowSettings(); });
	}

	void ViewerContextMenu::Zoomed(bool fullSize)
	{
		m_zoomFitImage->Check(false);
		m_zoomFullSize->Check(fullSize);
	}

	void ViewerContextMenu::FitImage()
	{
		m_zoomFitImage->Check(true);
		m_zoomFullSize->Check(false);
	}

	ViewerContextMenu::ViewerContextMenu() {}

	wxMenu* ViewerContextMenu::DoAddSubMenu(StringID label)
	{
		auto menu = new wxMenu();
		AppendSubMenu(menu, Win::GetStringWx(label));
		menu->Bind(wxEVT_MENU, [=](wxCommandEvent &evt) { OnMenu(evt); });
		return menu;
	}

	ViewerContextMenu::~ViewerContextMenu()
	{

	}

	void ViewerContextMenu::OnMenu(wxCommandEvent &evt)
	{
		m_menuMap.Execute(evt.GetId());
	}
}
