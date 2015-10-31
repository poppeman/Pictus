#include "viewer_contextmenu.h"
#include "res_viewer.h"
#include "viewer.h"
#include <VersionHelpers.h>

namespace App {
	using namespace Win;

	void ViewerContextMenu::Construct(Viewer* v)
	{
		m_viewer = v;

		m_menuMap.AddAction(m_menu.AddItem(SIDMenuOpen), [=]() { v->OpenFolder(); });

		auto pZoom(m_menu.AddSubMenu(SIDMenuZoom));
		m_idZoomFitImage = pZoom->AddItem(SIDActionZoomFitImage);
		m_menuMap.AddAction(m_idZoomFitImage, [=]() { v->ZoomMode(ZoomFitImage); });
		m_idZoomFullSize = pZoom->AddItem(SIDActionZoomFullSize);
		m_menuMap.AddAction(m_idZoomFullSize, [=]() { v->ZoomMode(ZoomFullSize); });

		pZoom->AddMenuSeparator();
		m_menuMap.AddAction(pZoom->AddItem(SIDActionZoomIn), [=]() { v->ZoomIn(); });
		m_menuMap.AddAction(pZoom->AddItem(SIDActionZoomOut), [=]() { v->ZoomOut(); });

		auto pRot(m_menu.AddSubMenu(SIDMenuOrientation));
		m_menuMap.AddAction(pRot->AddItem(SIDMenuOrientationNoRotation), [=]() { v->Rotate(Filter::RotationAngle::RotateDefault); });
		m_menuMap.AddAction(pRot->AddItem(SIDMenuOrientationMirror), [=]() { v->Rotate(Filter::RotationAngle::FlipX); });
		m_menuMap.AddAction(pRot->AddItem(SIDMenuOrientationFlip), [=]() { v->Rotate(Filter::RotationAngle::FlipY); });
		m_menuMap.AddAction(pRot->AddItem(SIDMenuOrientationRotate90), [=]() { v->Rotate(Filter::RotationAngle::Rotate90); });
		m_menuMap.AddAction(pRot->AddItem(SIDMenuOrientationRotate90Flip), [=]() { v->Rotate(Filter::RotationAngle::Rotate90FlipY); });
		m_menuMap.AddAction(pRot->AddItem(SIDMenuOrientationRotate180), [=]() { v->Rotate(Filter::RotationAngle::Rotate180); });
		m_menuMap.AddAction(pRot->AddItem(SIDMenuOrientationRotate270), [=]() { v->Rotate(Filter::RotationAngle::Rotate270); });
		m_menuMap.AddAction(pRot->AddItem(SIDMenuOrientationRotate270Flip), [=]() { v->Rotate(Filter::RotationAngle::Rotate270FlipY); });

		auto pSort(m_menu.AddSubMenu(SIDMenuSortBy));
		m_menuMap.AddAction(pSort->AddItem(SIDMenuSortByFilename), [=]() { v->Sort(Img::Cacher::SortMethod::SortFilename); });
		m_menuMap.AddAction(pSort->AddItem(SIDMenuSortByDateModified), [=]() { v->Sort(Img::Cacher::SortMethod::SortDateModified); });
		m_menuMap.AddAction(pSort->AddItem(SIDMenuSortByDateCreated), [=]() { v->Sort(Img::Cacher::SortMethod::SortDateCreated); });

		auto pWall(m_menu.AddSubMenu(SIDMenuSetWallpaper));

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
		m_menuMap.AddAction(pWall->AddItem(SIDMenuSetWallpaperTile), [=]() { v->ApplyWallpaper(Win::Wallpaper::Mode::Tile); });

		m_menuMap.AddAction(m_menu.AddItem(SIDMenuShowExplorer), [=]() { v->OpenDirectoryInExplorer(); });
		m_menuMap.AddAction(m_menu.AddItem(SIDMenuAdjust), [=]() { v->ShowAdjust(); });
		m_menuMap.AddAction(m_menu.AddItem(SIDMenuRename), [=]() { v->RenameCurrent(); });
		m_menuMap.AddAction(m_menu.AddItem(SIDMenuSettings), [=]() { v->ShowSettings(); });
	}

	void ViewerContextMenu::Display(Geom::PointInt pos)
	{
		m_menuMap.Execute(m_menu.Display(m_viewer, pos));
	}

	void ViewerContextMenu::Zoomed(bool fullSize)
	{
		m_menu.CheckMenuItem(m_idZoomFitImage, false);
		m_menu.CheckMenuItem(m_idZoomFullSize, fullSize);
	}

	void ViewerContextMenu::FitImage()
	{
		m_menu.CheckMenuItem(m_idZoomFitImage, true);
		m_menu.CheckMenuItem(m_idZoomFullSize, false);
	}

	ViewerContextMenu::ViewerContextMenu():
		m_viewer{ 0 }
	{}
}
