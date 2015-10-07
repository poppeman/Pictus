#include "StdAfx.h"
#include "viewer_contextmenu.h"
#include "res_viewer.h"
#include "viewer.h"
#include <VersionHelpers.h>

namespace App {
	using namespace Win;

	void ViewerContextMenu::Construct(Viewer* v) {
		m_viewer = v;

		m_menu.AddItem(ID_OPEN, SIDMenuOpen);
		Menu::Ptr pZoom(m_menu.AddSubMenu(SIDMenuZoom));
		pZoom->AddItem(ID_ZOOM_FITIMAGE, SIDActionZoomFitImage);
		pZoom->AddItem(ID_ZOOM_FULLSIZE, SIDActionZoomFullSize);
		pZoom->AddMenuSeparator();
		pZoom->AddItem(ID_ZOOM_ZOOMIN, SIDActionZoomIn);
		pZoom->AddItem(ID_ZOOM_ZOOMOUT, SIDActionZoomOut);

		Menu::Ptr pRot(m_menu.AddSubMenu(SIDMenuOrientation));
		pRot->AddItem(ID_ROTATE_DEFAULT, SIDMenuOrientationNoRotation);
		pRot->AddItem(ID_ROTATE_FLIPX, SIDMenuOrientationMirror);
		pRot->AddItem(ID_ROTATE_FLIPY, SIDMenuOrientationFlip);
		pRot->AddItem(ID_ROTATE_90, SIDMenuOrientationRotate90);
		pRot->AddItem(ID_ROTATE_180, SIDMenuOrientationRotate180);
		pRot->AddItem(ID_ROTATE_270, SIDMenuOrientationRotate270);

		Menu::Ptr pSort(m_menu.AddSubMenu(SIDMenuSortBy));
		pSort->AddItem(ID_SORT_FILENAME, SIDMenuSortByFilename);
		pSort->AddItem(ID_SORT_DATEMODIFIED, SIDMenuSortByDateModified);
		pSort->AddItem(ID_SORT_DATEACCESSED, SIDMenuSortByDateAccessed);
		pSort->AddItem(ID_SORT_DATECREATED, SIDMenuSortByDateCreated);

		Menu::Ptr pWall(m_menu.AddSubMenu(SIDMenuSetWallpaper));

		if (IsWindows7OrGreater()) {
			pWall->AddItem(ID_SETWALLPAPER_CROPFILL, SIDMenuSetWallpaperCropFill);
			pWall->AddItem(ID_SETWALLPAPER_PADFILL, SIDMenuSetWallpaperPadFill);
		}

		if (IsWindows8OrGreater()) {
			pWall->AddItem(ID_SETWALLPAPER_SPAN, SIDMenuSetWallpaperSpan);
		}

		pWall->AddItem(ID_SETWALLPAPER_STRETCH, SIDMenuSetWallpaperStretch);
		pWall->AddItem(ID_SETWALLPAPER_CENTER, SIDMenuSetWallpaperCenter);
		pWall->AddItem(ID_SETWALLPAPER_TILE, SIDMenuSetWallpaperTile);

		m_menu.AddItem(ID_OPENDIRECTORY, SIDMenuShowExplorer);
		m_menu.AddItem(ID_ADJUST, SIDMenuAdjust);
		m_menu.AddItem(ID_RENAME, SIDMenuRename);
		m_menu.AddItem(ID_SETTINGS, SIDMenuSettings);

		m_menuMap.AddAction(ID_OPEN, [=]() { v->OpenFolder(); });
		m_menuMap.AddAction(ID_ZOOM_ZOOMIN, [=]() { v->ZoomIn(); });
		m_menuMap.AddAction(ID_ZOOM_ZOOMOUT, [=]() { v->ZoomOut(); });
		m_menuMap.AddAction(ID_ZOOM_FITIMAGE, [=]() { v->ZoomMode(ZoomFitImage); });
		m_menuMap.AddAction(ID_ZOOM_FULLSIZE, [=]() { v->ZoomMode(ZoomFullSize); });
		m_menuMap.AddAction(ID_SETTINGS, [=]() { v->ShowSettings(); });
		m_menuMap.AddAction(ID_OPENDIRECTORY, [=]() { v->OpenDirectoryInExplorer(); });
		m_menuMap.AddAction(ID_ADJUST, [=]() { v->ShowAdjust(); });
		m_menuMap.AddAction(ID_RENAME, [=]() { v->RenameCurrent(); });
		m_menuMap.AddAction(ID_ROTATE_DEFAULT, [=]() { v->Rotate(Filter::RotationAngle::RotateDefault); });
		m_menuMap.AddAction(ID_ROTATE_FLIPX, [=]() { v->Rotate(Filter::RotationAngle::FlipX); });
		m_menuMap.AddAction(ID_ROTATE_FLIPY, [=]() { v->Rotate(Filter::RotationAngle::FlipY); });
		m_menuMap.AddAction(ID_ROTATE_90, [=]() { v->Rotate(Filter::RotationAngle::Rotate90); });
		m_menuMap.AddAction(ID_ROTATE_180, [=]() { v->Rotate(Filter::RotationAngle::Rotate180); });
		m_menuMap.AddAction(ID_ROTATE_270, [=]() { v->Rotate(Filter::RotationAngle::Rotate270); });
		m_menuMap.AddAction(ID_SORT_FILENAME, [=]() { v->Sort(Img::Cacher::SortFilename); });
		m_menuMap.AddAction(ID_SORT_DATEACCESSED, [=]() { v->Sort(Img::Cacher::SortDateAccessed); });
		m_menuMap.AddAction(ID_SORT_DATECREATED, [=]() { v->Sort(Img::Cacher::SortDateCreated); });
		m_menuMap.AddAction(ID_SORT_DATEMODIFIED, [=]() { v->Sort(Img::Cacher::SortDateModified); });
		m_menuMap.AddAction(ID_SETWALLPAPER_CENTER, [=]() { v->ApplyWallpaper(Win::Wallpaper::Mode::Center); });
		m_menuMap.AddAction(ID_SETWALLPAPER_STRETCH, [=]() { v->ApplyWallpaper(Win::Wallpaper::Mode::Stretch); });
		m_menuMap.AddAction(ID_SETWALLPAPER_TILE, [=]() { v->ApplyWallpaper(Win::Wallpaper::Mode::Tile); });
		m_menuMap.AddAction(ID_SETWALLPAPER_PADFILL, [=] { v->ApplyWallpaper(Win::Wallpaper::Mode::FillPad); });
		m_menuMap.AddAction(ID_SETWALLPAPER_CROPFILL, [=] { v->ApplyWallpaper(Win::Wallpaper::Mode::FillCrop); });
		m_menuMap.AddAction(ID_SETWALLPAPER_SPAN, [=] { v->ApplyWallpaper(Win::Wallpaper::Mode::Span); });
	}

	void ViewerContextMenu::Display(Geom::PointInt pos) {
		m_menuMap.Execute(m_menu.Display(m_viewer, pos));
	}

	void ViewerContextMenu::Zoomed(bool fullSize) {
		m_menu.CheckMenuItem(ID_ZOOM_FITIMAGE, false);
		m_menu.CheckMenuItem(ID_ZOOM_FULLSIZE, fullSize);
	}

	void ViewerContextMenu::FitImage() {
		m_menu.CheckMenuItem(ID_ZOOM_FITIMAGE, true);
		m_menu.CheckMenuItem(ID_ZOOM_FULLSIZE, false);
	}

	ViewerContextMenu::ViewerContextMenu():m_viewer(0) {}
}
