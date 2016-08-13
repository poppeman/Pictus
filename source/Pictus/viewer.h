
#ifndef VIEWER_H
#define VIEWER_H

#include "orz/folder_monitor.h"

#include "imagecache.h"

#include "w32_rendertarget.h"

//#include "adjust.h"
#include "settings.h"
#include "viewport.h"

#include "wallpaper.h"

#include "illa/codecmgr.h"
#include "appreg.h"

//#include "viewer_contextmenu.h"
#include "viewer_keyboard.h"
#include "actionmap.h"

#include <random>

#include <ctime>
#include <wx/frame.h>

namespace App {
	class Viewer:public wxFrame, public Img::MessageReceiver {
	public:
		static const wchar_t* ClassName;
		static const wchar_t* AppTitle;

		//bool Show(bool doShow);
		void ActiveImage(Img::Image::Ptr pImage);

		bool Init(const std::string params);

		Viewer(Img::CodecFactoryStore* cfs, Reg::Settings config);
		~Viewer();

	private:
		bool m_attemptToShow;

		friend class ViewerContextMenu;
		friend class ViewerKeyboard;

		enum {
			KeyPanSize = 100,
			MinWindowWidth = 180,
			MinWindowHeight	= 180,
			FileSizeDivider = 10,
			ShiftSkipLength = 5,
		};

		enum StatusBarField {
			StatusZoom=0,
			StatusName,
			StatusResolution,
			StatusPosition,
			StatusProgress,
			StatusFileSize,
			StatusLastModified,
			StatusNumParts
		};

		enum RemoveOp {
			RemoveOnly,
			RemoveDelete,
			RemoveRecycle
		};

		enum {
			StatFieldZoomWidth =	60,
			StatFieldImageDimWidth= 100,
			StatFieldPosWidth =		80,
			StatFieldTimeWidth =	110,
			StatFieldFileSizeWidth=	80,
			StatFieldLastModified = 180,
		};

		void OnMouseStandardEvent(wxMouseEvent& e);
		void OnMouseDoubleEvent(wxMouseEvent& e);
		void OnSizeEvent(wxSizeEvent& e);
		void OnImageLoadEvent(wxCommandEvent& e);

		void AdjustChange(int brightness, int contrast, int gamma);
		void ShowAdjust();
		void ShowSettings();
		void OpenDirectoryInExplorer();
		void SettingsChanged();

		void UpdateViewportConfig();

		void Sort(Img::Cacher::SortMethod m);

		bool PerformOnWindowCreate();
		bool PerformOnClose();

		bool PerformOnSize(const Geom::SizeInt& sz);
		void OnMoveEvent(wxMoveEvent& e);
		//bool PerformOnDropFiles(const StringVector& files);

		void SetImageLocation(const std::string& path);

		//bool PerformOnCopyData(const COPYDATASTRUCT* pcds);
		void UpdateImageInformation();

		std::string UII_MemoryUsage(FileInt size);
		std::string UII_LoadProgress(Img::Image::Ptr image);
		std::string UII_ImageResolution(Img::Image::Ptr image);
		std::string UII_LastModified(std::time_t date);

		bool RecalculateViewportSize();

		void ZoomIn();
		void ZoomOut();
		void ZoomToggleFullSizeDefaultZoom();
		void ZoomDefault();

		void UpdateMemoryLimits();

		void ZoomMode(App::ZoomMode mode);

		enum ScreenMode {
			SM_Normal,
			SM_Fullscreen
		};

		void ViewportMode(ScreenMode newMode);
		ScreenMode ViewportMode() const;
		ScreenMode m_screenMode;

		void PanUp();
		void PanRight();
		void PanDown();
		void PanLeft();

		void Rotate(Filter::RotationAngle r);
		void RotateLeft();
		void RotateRight();

		void ToStart();
		void ImagePrev(unsigned numSteps = 1);
		void ImageNext(unsigned numSteps = 1);
		void ImageRandom();
		void ToEnd();

		void RemoveImage(RemoveOp op);

		Geom::SizeInt calculateCappedImageSize( const Geom::SizeInt& imageSize, const Geom::SizeInt &windowEdges );

		void ImageChanged();

		bool ApplyWallpaper(Win::Wallpaper::Mode orientation);

		void RenameCurrent();
		void OpenFolder();

		void OnLoadMessage(Img::MessageReceiver::LoadMessage s, Img::Image* pImage, const std::string& desc);
		void FolderEvent(IO::FileEvent e);

		bool CopyToClipboard();

		// Disable copy and assignment
		Viewer(const Viewer&);

		void ShowContextMenu(Win::MouseEvent e);
		Viewer& operator=(const Viewer& rhs)=delete;

		struct CacheNotification {
			Img::Image* image;
			Img::MessageReceiver::LoadMessage message;
			std::string desc;
		};

		void AnchorTL(const Geom::PointInt& pt);
		Geom::PointInt AnchorTL() const;
		void AnchorCenter(const Geom::PointInt& pt);
		Geom::PointInt AnchorCenter() const;

		void AddNotification(const IO::FileEvent& notification);
		void HandleFolderNotification();

		void AddNotification(const CacheNotification& notification);
		void HandleCacheNotification();

		Geom::PointInt calculateWindowTopLeft( ResizePositionMethod method, const Geom::SizeInt &newSize );
		Geom::SizeInt calculateImageSize( ResizeBehaviour mode, float xratio, float yratio, const Geom::SizeInt &imageSize, const Geom::SizeInt &windowEdges );

		void ToggleFullscreenMode();

		std::string m_sDirectory;

		boost::signals2::connection m_lang;

		//LONG_PTR m_previousWindowStyle;
		Geom::RectInt m_previousWindowRegion;
		Geom::RectInt m_previousNonMaximizedWindowRegion;

		// Cacher
		Img::Cacher m_cacher;
		IO::Folder m_folder;
		IO::FolderMonitor m_folderMonitor;

		// Window size and such
		bool m_doMaximize;

		// Settings, adjust, stuff like that
		std::shared_ptr<Settings> m_settings;
		//Adjust m_adjust;

		ViewPort m_viewPort;

		wxStatusBar* m_statusBar;

		std::mutex m_mutexNotification;

		typedef std::list<CacheNotification> CacheNotificationList;
		CacheNotificationList m_cacheNotifications;
		typedef std::list<IO::FileEvent> FolderNotificationList;
		FolderNotificationList m_folderNotifications;

		Img::CodecFactoryStore* m_codecs;

		//ViewerContextMenu m_contextMenu;
		ViewerKeyboard m_keys;

		//HANDLE m_singleMutex;

		typedef ActionMapParam<MouseAction, Win::MouseEvent> MouseActionMap;
		MouseActionMap m_mouseMap;

		std::default_random_engine m_random;

		Reg::Settings m_cfg;

		Geom::PointInt PositionScreen();
		Geom::PointInt CenterPositionScreen();
		Geom::RectInt ClientRect();
		void AlwaysOnTop(bool doAlwaysOnTop);

		unsigned int DisplayFromPointFallback(Geom::PointInt position);

		Geom::RectInt m_normalRect;

		wxDECLARE_EVENT_TABLE();
	};
}

#endif
