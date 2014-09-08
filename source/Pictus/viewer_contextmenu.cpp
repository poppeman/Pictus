#include "StdAfx.h"
#include "viewer_contextmenu.h"
#include "res_viewer.h"
#include "viewer.h"

namespace App {
	using namespace Win;

	void ViewerContextMenu::Construct(Viewer* v) {
		m_viewer = v;

		m_menu.AddItem(ID_OPEN, SIDOpen);
		Menu::Ptr pZoom(m_menu.AddSubMenu(SIDZoom));
		pZoom->AddItem(ID_ZOOM_FITIMAGE, SIDZoomFitImage);
		pZoom->AddItem(ID_ZOOM_FULLSIZE, SIDZoomFullSize);
		pZoom->AddMenuSeparator();
		pZoom->AddItem(ID_ZOOM_ZOOMIN, SIDMenuZoomIn);
		pZoom->AddItem(ID_ZOOM_ZOOMOUT, SIDMenuZoomOut);

		Menu::Ptr pRot(m_menu.AddSubMenu(SIDOrientation));
		pRot->AddItem(ID_ROTATE_DEFAULT, SIDOrientationDefault);
		pRot->AddItem(ID_ROTATE_FLIPX, SIDOrientationFlipX);
		pRot->AddItem(ID_ROTATE_FLIPY, SIDOrientationFlipY);
		pRot->AddItem(ID_ROTATE_90, SIDOrientation90);
		pRot->AddItem(ID_ROTATE_180, SIDOrientation180);
		pRot->AddItem(ID_ROTATE_270, SIDOrientation270);

		Menu::Ptr pSort(m_menu.AddSubMenu(SIDMenuSort));
		pSort->AddItem(ID_SORT_FILENAME, SIDMenuSortFilename);
		pSort->AddItem(ID_SORT_DATEMODIFIED, SIDMenuSortDateModified);
		pSort->AddItem(ID_SORT_DATEACCESSED, SIDMenuSortDateAccessed);
		pSort->AddItem(ID_SORT_DATECREATED, SIDMenuSortDateCreated);

		Menu::Ptr pWall(m_menu.AddSubMenu(SIDSetWallpaper));
		pWall->AddItem(ID_SETWALLPAPER_STRETCH, SIDWPStretch);
		pWall->AddItem(ID_SETWALLPAPER_CENTER, SIDWPCenter);
		pWall->AddItem(ID_SETWALLPAPER_TILE, SIDWPTile);

		m_menu.AddItem(ID_OPENDIRECTORY, SIDOpenDirectory);
		m_menu.AddItem(ID_ADJUST, SIDAdjust);
		m_menu.AddItem(ID_RENAME, SIDMenuRename);
		m_menu.AddItem(ID_SETTINGS, SIDSettings);

		m_menuMap.AddAction(ID_OPEN, [=]() { v->OpenFolder(); });
		m_menuMap.AddAction(ID_ZOOM_ZOOMIN, [=]() { v->ZoomIn(); });
		m_menuMap.AddAction(ID_ZOOM_ZOOMOUT, [=]() { v->ZoomOut(); });
		m_menuMap.AddAction(ID_ZOOM_FITIMAGE, [=]() { v->ZoomMode(ZoomFitImage); });
		m_menuMap.AddAction(ID_ZOOM_FULLSIZE, [=]() { v->ZoomFullSize(); });
		m_menuMap.AddAction(ID_SETTINGS, [=]() { v->ShowSettings(); });
		m_menuMap.AddAction(ID_OPENDIRECTORY, [=]() { v->OpenDirectoryInExplorer(); });
		m_menuMap.AddAction(ID_ADJUST, [=]() { v->ShowAdjust(); });
		m_menuMap.AddAction(ID_RENAME, [=]() { v->RenameCurrent(); });
		m_menuMap.AddAction(ID_ROTATE_DEFAULT, [=]() { v->Rotate(Filter::RotateDefault); });
		m_menuMap.AddAction(ID_ROTATE_FLIPX, [=]() { v->Rotate(Filter::FlipX); });
		m_menuMap.AddAction(ID_ROTATE_FLIPY, [=]() { v->Rotate(Filter::FlipY); });
		m_menuMap.AddAction(ID_ROTATE_90, [=]() { v->Rotate(Filter::Rotate90); });
		m_menuMap.AddAction(ID_ROTATE_180, [=]() { v->Rotate(Filter::Rotate180); });
		m_menuMap.AddAction(ID_ROTATE_270, [=]() { v->Rotate(Filter::Rotate270); });
		m_menuMap.AddAction(ID_SORT_FILENAME, [=]() { v->Sort(Img::Cacher::SortFilename); });
		m_menuMap.AddAction(ID_SORT_DATEACCESSED, [=]() { v->Sort(Img::Cacher::SortDateAccessed); });
		m_menuMap.AddAction(ID_SORT_DATECREATED, [=]() { v->Sort(Img::Cacher::SortDateCreated); });
		m_menuMap.AddAction(ID_SORT_DATEMODIFIED, [=]() { v->Sort(Img::Cacher::SortDateModified); });
		m_menuMap.AddAction(ID_SETWALLPAPER_CENTER, [=]() { v->ApplyWallpaper(Win::Wallpaper::WPCenter); });
		m_menuMap.AddAction(ID_SETWALLPAPER_STRETCH, [=]() { v->ApplyWallpaper(Win::Wallpaper::WPStretch); });
		m_menuMap.AddAction(ID_SETWALLPAPER_TILE, [=]() { v->ApplyWallpaper(Win::Wallpaper::WPTile); });

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
