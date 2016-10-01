#include "orz/stream_file.h"
#include "orz/fileops.h"
#include "orz/intl.h"
#include "orz/sysinfo.h"

#include "imagecache.h"
#include "illa/surfacemgr.h"

#include "rename.h"

#include "registry.h"

#include "getevent.h"
#include "viewer.h"

#include "config.h"

#include "filterstring.h"
#include "builder_viewport.h"

#include "clipboard.h"
#include "filemanager.h"

#include <boost/date_time/c_local_time_adjustor.hpp>
#include <boost/format.hpp>
#include <boost/scoped_array.hpp>
#include <wx/display.h>
#include <wx/msgdlg.h>
#include <wx/filedlg.h>
#include <wx/clipbrd.h>

const wchar_t* App::Viewer::AppTitle = L"Pictus";

wxDEFINE_EVENT(ImageLoadEvent, wxCommandEvent);
wxDEFINE_EVENT(FolderEvent, wxCommandEvent);

namespace App {
	using namespace Win;
	using namespace Intl;

	using namespace Geom;

	wxBEGIN_EVENT_TABLE(Viewer, wxFrame)
		EVT_LEFT_DOWN(Viewer::OnMouseStandardEvent)
		EVT_LEFT_DCLICK(Viewer::OnMouseDoubleEvent)
		EVT_MIDDLE_DOWN(Viewer::OnMouseStandardEvent)
		EVT_MIDDLE_DCLICK(Viewer::OnMouseDoubleEvent)
		EVT_RIGHT_DOWN(Viewer::OnMouseStandardEvent)
		EVT_RIGHT_DCLICK(Viewer::OnMouseDoubleEvent)
		EVT_MOUSEWHEEL(Viewer::OnMouseStandardEvent)
		EVT_SIZE(Viewer::OnSizeEvent)
		EVT_SIZING(Viewer::OnSizeEvent)
		EVT_MOVE(Viewer::OnMoveEvent)
		EVT_COMMAND(wxID_ANY, ImageLoadEvent, Viewer::OnImageLoadEvent)
	wxEND_EVENT_TABLE()

	void Viewer::OnMouseStandardEvent(wxMouseEvent &e)
	{
		if (!m_mouseMap.Execute(MouseStandardEvent(e, m_cfg.Mouse), e))
		{
			e.ResumePropagation(1);
			e.Skip();
		}
	}

	void Viewer::OnMouseDoubleEvent(wxMouseEvent &e)
	{
		auto ev = MouseDblEvent(e, m_cfg.Mouse);
		if (ev == MouseAction::MouseUndefined || ev == MouseAction::MouseDisable)
		{
			ev = MouseStandardEvent(e, m_cfg.Mouse);
		}

		if (!m_mouseMap.Execute(ev, e))
		{
			e.ResumePropagation(1);
			e.Skip();
		}
	}

	void Viewer::OnSizeEvent(wxSizeEvent &e)
	{
		if (!IsMaximized() && !IsIconized())
		{
			m_normalRect = wxToRect(GetRect());
		}

		e.Skip();
	}

	void Viewer::OnImageLoadEvent(wxCommandEvent &e)
	{
		std::unique_lock<std::mutex> l(m_mutexNotification);
		if (m_cacheNotifications.empty()) {
			DO_THROW(Err::CriticalError, "Notification queue is empty.");
		}

		CacheNotification notification = m_cacheNotifications.front();
		m_cacheNotifications.pop_front();
		l.unlock();

		if (notification.message == Img::MessageReceiver::LoadErrorCritical) {
			/*m_exceptionDescription = notification.desc;
			m_exceptionOcurred = true;
			PostMessage(Handle(), WM_CLOSE, 0, 0);*/
		}
		if (notification.image == m_viewPort.Image().get()) {
			switch (notification.message) {
			case Img::MessageReceiver::LoadDone:
				m_viewPort.ImageUpdated();
				UpdateImageInformation();
				m_viewPort.Refresh();
				break;
			case Img::MessageReceiver::LoadAllocated:
				m_viewPort.ImageUpdated();
				ImageChanged();
				break;
			case Img::MessageReceiver::LoadHeader:
				UpdateImageInformation();
				break;
			case Img::MessageReceiver::LoadErrorImage:
				UpdateImageInformation();
				break;
			}

			if (m_attemptToShow && notification.message >= Img::MessageReceiver::LoadAllocated) {
				Show(true);
			}
		}
	}

	Viewer::Viewer(Img::CodecFactoryStore* cfs, const Reg::Settings& cfg):
		m_attemptToShow{ false },
		m_screenMode{ SM_Normal },
		m_doMaximize{ false },
		m_viewPort{ this },
		m_statusBar{ nullptr },
		m_codecs{ cfs },
		m_dropTarget{ nullptr },
		m_cfg( cfg ),
		m_userInitiatedMove{ true }
	{
		Bind(FolderEvent, &Viewer::OnFolderEvent, this);
	}

	Viewer::~Viewer() {
		m_folderMonitor.Close();
		m_cacher.Stop();
	}

	/*bool Viewer::Show(bool doShow) {
		auto img = m_viewPort.Image();
		auto allocated = IsPositive(m_viewPort.OptimalViewportSize());
		auto defective = img?!img->IsHeaderInformationValid() && img->IsFinished():true;
		ImageChanged();
		if (m_doMaximize == false && !allocated && !defective) {
			m_attemptToShow = doShow;
			return true;
		}

		ShowWindow(Handle(), (doShow ? (m_doMaximize ? SW_MAXIMIZE : SW_SHOW) : SW_HIDE));
		if (doShow) {
			m_doMaximize = false;
		}
		m_attemptToShow = false;
		return true;
	}*/

	Img::Image::Ptr Viewer::ActiveImage() const
	{
		return m_viewPort.Image();
	}

	void Viewer::ActiveImage(Img::Image::Ptr pImage) {
		if (m_viewPort.Image() == pImage) {
			return;
		}

		m_viewPort.Image(pImage);

		if (m_cfg.View.ResetZoom) {
			ZoomMode(m_cfg.View.DefaultZoomMode);
			UpdateImageInformation();
		}
		else {
			ImageChanged();
		}
	}

	void Viewer::CreateWindow()
	{
		auto winSize = wxDefaultSize;
		auto winPos = wxDefaultPosition;

		auto displayIndex = wxDisplay::GetFromPoint({ m_cfg.View.WindowPosX, m_cfg.View.WindowPosY });
		if (displayIndex != wxNOT_FOUND && displayIndex >= 0)
		{
			wxDisplay display(static_cast<unsigned int>(displayIndex));
			auto displayGeometry = display.GetClientArea();

			int w = Util::Constrain<int>(MinWindowWidth, m_cfg.View.WindowSizeWidth, displayGeometry.GetWidth());
			int h = Util::Constrain<int>(MinWindowHeight, m_cfg.View.WindowSizeHeight, displayGeometry.GetHeight());

			int x = Util::Constrain<int>(displayGeometry.GetLeft(), m_cfg.View.WindowPosX, displayGeometry.GetRight() - w);
			int y = Util::Constrain<int>(displayGeometry.GetTop(), m_cfg.View.WindowPosY, displayGeometry.GetBottom() - h);

			winSize = { w, h };
			winPos = { x, y };
		}

		Create(nullptr, -1, L"Pictus", winPos, winSize, wxDEFAULT_FRAME_STYLE);
	}


	bool Viewer::Init(const std::string params)
	{
		m_dropTarget = new DropTarget(this);
		m_normalRect = wxToRect(GetRect());

		Bind(wxEVT_CLOSE_WINDOW, [&](wxCloseEvent& evt) { PerformOnClose(); evt.Skip(); });

		ViewportBuilder b;
		b.BuildViewport(m_viewPort, this, m_cfg);

		m_adjust = std::make_shared<Adjust>(this);
		m_adjust->OnChange.connect([this](int a, int b, int c) { AdjustChange(a, b, c); });

		m_mouseMap.AddAction(MouseFullscreen, [this](Win::MouseEvent) { ToggleFullscreenMode(); });
		m_mouseMap.AddAction(MouseToggleFullSizeDefaultZoom, [&](Win::MouseEvent) { ZoomToggleFullSizeDefaultZoom(); });
		m_mouseMap.AddAction(MouseNextImage, [this](Win::MouseEvent) { ImageNext(1); });
		m_mouseMap.AddAction(MousePrevImage, [this](Win::MouseEvent) { ImagePrev(1); });
		m_mouseMap.AddAction(MouseZoomIn, [this](Win::MouseEvent) { ZoomIn(); });
		m_mouseMap.AddAction(MouseZoomOut, [this](Win::MouseEvent) { ZoomOut(); });
		m_mouseMap.AddAction(MouseContext, [this](Win::MouseEvent e) { ShowContextMenu(e); });
		m_mouseMap.AddAction(MouseRotateLeft, [this](Win::MouseEvent) { RotateLeft(); });
		m_mouseMap.AddAction(MouseRotateRight, [this](Win::MouseEvent) { RotateRight(); });

		m_contextMenu.Construct(this);
		m_keys.Construct(this);
		m_keys.SetBindings(m_cfg.Keyboard);

		m_lang = Intl::OnLanguageChanged.connect([&]() { UpdateImageInformation(); });

		m_settings = std::make_shared<Settings>(this, m_cfg);
		m_settings->OnSettingsChanged.connect([&](Reg::Settings newSettings) {
			m_cfg = newSettings;
			SettingsChanged();
		});

		m_cacher.SetCodecFactoryStore(m_codecs);

		UpdateViewportConfig();

		// Apply some settings that can't be set automatically
		UpdateMemoryLimits();

		m_cacher.MessageTarget(this);
		// TODO: FIXME: This is very wrong, should be set to something that isn't hardcoded.
		m_cacher.SetMaximumResolutionHint(Geom::SizeInt(25600, 25600));
		m_folderMonitor.OnEvent.connect([this](IO::FileEvent e) { AddNotification(e); });

		m_cacher.WrapAround(m_cfg.View.BrowseWrapAround);

		AlwaysOnTop(m_cfg.View.AlwaysOnTop);

		ZoomMode(m_cfg.View.DefaultZoomMode);

		if (m_cfg.View.Maximized) {
			m_doMaximize = true;
		}

		m_viewPort.Init();
		m_statusBar = CreateStatusBar(7);

		int widths[] = {
			StatFieldZoomWidth,
			-1,
			StatFieldImageDimWidth,
			StatFieldPosWidth,
			StatFieldTimeWidth,
			StatFieldFileSizeWidth,
			StatFieldLastModified
		};

		m_statusBar->SetStatusWidths(7, widths);

		m_statusBar->Show(m_cfg.View.ShowStatusBar);

		SetImageLocation(params);

		SetDropTarget(m_dropTarget);

		return true;
	}

	bool Viewer::PerformOnClose() {
		SizeInt sz	= wxToSize(GetSize());
		PointInt pt	= PositionScreen();

		// Store un-maximized values instead!
		if(m_screenMode == SM_Fullscreen || IsMaximized())
		{
			sz = m_normalRect.Dimensions();
			pt = m_normalRect.TopLeft();
		}

		// Don't want to save bogus values!
		if (!IsIconized())
		{
			m_cfg.View.WindowSizeWidth = sz.Width;
			m_cfg.View.WindowSizeHeight = sz.Height;
			m_cfg.View.WindowPosX = pt.X;
			m_cfg.View.WindowPosY = pt.Y;
		}

		m_cfg.View.Maximized = IsMaximized();

		Reg::Save(cg_SettingsLocation, m_cfg);

		return true;
	}

	bool Viewer::CopyToClipboard() {
		return ImageToClipboard(m_viewPort.Image(), m_viewPort.Properties());
	}

	void Viewer::OnMoveEvent(wxMoveEvent& e)
	{
		if(!IsMaximized() && !IsIconized())
		{
			m_normalRect = wxToRect(GetRect());
		}

		if (m_userInitiatedMove)
		{
			AnchorTL(PositionScreen());
			AnchorCenter(PositionScreen() + RoundCast(wxToSize(GetSize()) * 0.5f));
		}
	}

	void Viewer::AnchorTL(const PointInt& pt) {
		m_cfg.View.WindowAnchorTLX = pt.X;
		m_cfg.View.WindowAnchorTLY = pt.Y;
	}

	PointInt Viewer::AnchorTL() const {
		return{ m_cfg.View.WindowAnchorTLX, m_cfg.View.WindowAnchorTLY };
	}

	void Viewer::AnchorCenter(const PointInt& pt) {
		m_cfg.View.WindowAnchorCenterX = pt.X;
		m_cfg.View.WindowAnchorCenterY = pt.Y;
	}

	PointInt Viewer::AnchorCenter() const {
		return{ m_cfg.View.WindowAnchorCenterX, m_cfg.View.WindowAnchorCenterY };
	}

/*	bool Viewer::PerformOnCopyData(const COPYDATASTRUCT* pcds) {
		if (pcds->dwData != 0) return false;

		SetImageLocation(WStringToUTF8(reinterpret_cast<wchar_t*>(pcds->lpData)));
		return true;
	}*/

	bool Viewer::ApplyWallpaper(Win::Wallpaper::Mode orientation) {
#ifdef _WIN32
		return Wallpaper::ApplyWallpaper(m_cacher.CurrentImageFilename(), orientation);
#else
		return false;
#endif
	}

	void Viewer::UpdateImageInformation() {
		if (m_statusBar == nullptr)
		{
			return;
		}

		if (ViewportMode() == SM_Fullscreen) {
			m_statusBar->Show(false);
			return;
		}

		Img::Image::Ptr image(m_viewPort.Image());
		m_statusBar->SetStatusText(UII_LoadProgress(image), StatusProgress);
		m_statusBar->SetStatusText(UII_ImageResolution(image), StatusResolution);
		SetTitle(wxString::FromUTF8(((image ? IO::GetFile(m_cacher.CurrentImageFilename()) + " - ":"") + WStringToUTF8(AppTitle)).c_str()));

		if (image == nullptr) {
			for (int i = 0; i < StatusNumParts; ++i) {
				m_statusBar->SetStatusText("", 0);
			}
		}
		else
		{
			m_statusBar->SetStatusText(IO::GetFile(m_cacher.CurrentImageFilename()), StatusName);
			m_statusBar->SetStatusText(ToAString(m_cacher.CurrentImageIndex() + 1) + "\\" + ToAString(m_cacher.ImageCount()), StatusPosition);
			m_statusBar->SetStatusText(UII_MemoryUsage(m_cacher.CurrentImageFileSize()), StatusFileSize);

			if (image->IsHeaderInformationValid() == false && image->IsFinished())
			{
				m_statusBar->SetStatusText("", StatusZoom);
			}
			else
			{
				m_statusBar->SetStatusText(ToAString(RoundCast(m_viewPort.ZoomLevel() * 100.0f)) + "%", StatusZoom);
				m_statusBar->SetStatusText(UII_LastModified(m_cacher.CurrentImageLastModifiedDate()), StatusLastModified);
			}
		}
	}

	std::string Viewer::UII_MemoryUsage(FileInt size) {
		FileInt alloc = size * FileSizeDivider;
		FileInt kbSize = alloc >> 10;
		FileInt mbSize = kbSize >> 10;
		FileInt gbSize = mbSize >> 10;

		if (gbSize > FileSizeDivider) {
			return ToAString(gbSize / (float)FileSizeDivider) + " " + GetString(SIDUnitGB);
		}
		else if (mbSize > FileSizeDivider) {
			return ToAString(mbSize / (float)FileSizeDivider) + " " + GetString(SIDUnitMB);
		}
		else if (kbSize > FileSizeDivider) {
			return ToAString(kbSize / (float)FileSizeDivider) + " " + GetString(SIDUnitKB);
		}
		else {
			return ToAString(alloc / (float)FileSizeDivider) + " " + GetString(SIDUnitB);
		}
	}

	std::string Viewer::UII_LoadProgress(Img::Image::Ptr image) {
		if (image == nullptr || (image->IsHeaderInformationValid() == false && image->IsFinished())) {
			return "";
		}

		if (image->IsFinished()) {
			return (boost::format(GetString(SIDStatusbarLoadTime)) % ToAString(image->LoadTime() / 1000.0f)).str();
		}

		return GetString(SIDStatusbarLoading);
	}

	std::string Viewer::UII_ImageResolution(Img::Image::Ptr image) {
		if(!image || (image->IsHeaderInformationValid() == false && image->IsFinished()))
			return "";

		SizeInt sz = image->GetTransformedSize();
		if(IsPositive(sz))
			return ToAString(sz.Width) + "x" + ToAString(sz.Height);

		return "";
	}

	void Viewer::ZoomIn() {
		m_viewPort.ZoomIn();
		m_contextMenu.Zoomed(std::fabs(m_viewPort.ZoomLevel() - 1.0f) < std::numeric_limits<float>::epsilon());
		ImageChanged();
		UpdateImageInformation();
	}

	void Viewer::ZoomOut() {
		m_viewPort.ZoomOut();
		m_contextMenu.Zoomed(std::fabs(m_viewPort.ZoomLevel() - 1.0) < std::numeric_limits<float>::epsilon());
		ImageChanged();
		UpdateImageInformation();
	}

	void Viewer::ZoomToggleFullSizeDefaultZoom() {
		if (m_viewPort.ZoomMode() != m_cfg.View.DefaultZoomMode) {
			ZoomMode(m_cfg.View.DefaultZoomMode);
		}
		else {
			ZoomMode(ZoomFullSize);
		}
	}

	void Viewer::ZoomMode(App::ZoomMode mode) {
		m_viewPort.ZoomMode(mode);
		switch (mode) {
			case App::ZoomFitImage:
				m_contextMenu.FitImage();
				break;

			case App::ZoomFullSize:
			case App::ZoomFree:
				m_contextMenu.Zoomed(true);
				break;
		}

		ImageChanged();
		UpdateImageInformation();
	}

	void Viewer::ZoomDefault() {
		ZoomMode(m_cfg.View.DefaultZoomMode);
	}

	void Viewer::ToggleFullscreenMode() {
		if (m_screenMode == Viewer::SM_Normal)
			ViewportMode(Viewer::SM_Fullscreen);
		else
			ViewportMode(Viewer::SM_Normal);
	}

	void Viewer::ViewportMode(ScreenMode newMode)
	{
		if (m_screenMode == newMode)
		{
			return;
		}

		m_screenMode = newMode;

		if (newMode == Viewer::SM_Fullscreen)
		{
			m_userInitiatedMove = false;
			wxDisplay mon(DisplayFromPointFallback(PositionScreen()));

			m_statusBar->Show(false);
			m_previousWindowStyle = GetWindowStyle();
			SetWindowStyleFlag(wxBORDER_NONE);
			m_previousWindowRegion = wxToRect(GetRect());
			auto monRegion = wxToRect(mon.GetClientArea());

			SetSize(monRegion.Left(), monRegion.Top(), monRegion.Width(), monRegion.Height());
			m_viewPort.ActiveCursorMode(ViewPort::CursorHideAutomatic);
			Raise();
			m_userInitiatedMove = true;
		}
		else
		{
			m_userInitiatedMove = false;
			SetWindowStyleFlag(m_previousWindowStyle);
			m_statusBar->Show(m_cfg.View.ShowStatusBar);
			SetSize(m_previousWindowRegion.Left(), m_previousWindowRegion.Top(), m_previousWindowRegion.Width(), m_previousWindowRegion.Height());
			m_viewPort.ActiveCursorMode(ViewPort::CursorShow);
			m_userInitiatedMove = true;
		}

		// Make sure that the settings and adjust dialogs are on top (if running)
		if (m_settings->IsVisible())
		{
			ShowSettings();
		}

		if (m_adjust->IsVisible())
		{
			ShowAdjust();
		}

		ImageChanged();
		UpdateImageInformation();
	}

	Viewer::ScreenMode Viewer::ViewportMode() const {
		return m_screenMode;
	}

	void Viewer::PanVertical(int length) {
		m_viewPort.Pan(SizeInt(0, length));
	}

	void Viewer::PanHorizontal(int length) {
		m_viewPort.Pan(SizeInt(length, 0));
	}

	void Viewer::Rotate(Filter::RotationAngle r) {
		m_viewPort.Rotate(r);

		UpdateImageInformation();
		ImageChanged();
		m_viewPort.ImageUpdated();
	}

	void Viewer::RotateLeft() {
		Rotate(Img::RotateLeft(m_viewPort.Rotation()));
	}

	void Viewer::RotateRight() {
		Rotate(Img::RotateRight(m_viewPort.Rotation()));
	}

	void Viewer::ToStart() {
		ActiveImage(m_cacher.FirstImage());
	}

	void Viewer::ImagePrev(unsigned toStep) {
		while (toStep-- > 0)
			m_cacher.PreviousImage();

		ActiveImage(m_cacher.CurrentImage());
	}

	void Viewer::ImageNext(unsigned toStep) {
		while (toStep-- > 0)
			m_cacher.NextImage();

		ActiveImage(m_cacher.CurrentImage());
	}

	void Viewer::ImageRandom() {

		size_t imageCount = m_cacher.ImageCount();
		if (imageCount <= 1) return;

		std::uniform_int_distribution<size_t> posDist(0, imageCount - 2);

		size_t pos = posDist(m_random);

		if (pos >= m_cacher.CurrentImageIndex()) pos++;

		ActiveImage(m_cacher.GotoImage(pos));
	}

	void Viewer::ToEnd() {
		ActiveImage(m_cacher.LastImage());
	}

	void Viewer::RenameCurrent() {
		auto image(m_viewPort.Image());
		if (image == nullptr) return;

		auto old_name(m_cacher.CurrentImageFilename());
		auto extension = IO::GetExtension(old_name);

		auto ren = new Rename(this, IO::GetTitle(old_name));

		if (ren->ShowModal())
		{
			auto new_name = IO::GetPath(old_name) + ren->Name() + "." + extension;
			auto reader = m_cacher.CurrentImageFileReader();
			if (reader == nullptr) {
				return;
			}
// TODO: Support on Linux
#ifdef _WIN32
			auto resulting_name = dynamic_cast<IO::StreamFile*>(reader->GetStream())->Rename(new_name, GetHandle());
			m_cacher.RenamedImage(old_name, resulting_name);
#endif
			UpdateImageInformation();
		}
	}


	void Viewer::RemoveImage(RemoveOp op) {
		auto image(m_viewPort.Image());
		if (image == nullptr)
		{
			return;
		}

		if (op == RemoveOnly)
		{
			ActiveImage(m_cacher.RemoveCurrentImage());
			return;
		}

		auto old_name(m_cacher.CurrentImageFilename());

		auto reader = m_cacher.CurrentImageFileReader();
		if (reader == nullptr)
		{
			return;
		}

// TODO: Support on Linux
#ifdef _WIN32
		auto f = dynamic_cast<IO::StreamFile*>(reader->GetStream());
		if (f->Delete((op == RemoveRecycle), GetHandle()))
		{
			ActiveImage(m_cacher.RemoveCurrentImage());
		}
#endif

		UpdateImageInformation();
	}

	void Viewer::OpenFolder() {
		FilterString s(m_codecs);
		wxFileDialog openDialog(
			this,
			Win::GetStringWx(SIDOpen),
			wxEmptyString,
			wxEmptyString,
			wxString::FromUTF8(s.GetFilterString().c_str()),
			wxFD_DEFAULT_STYLE,
			wxDefaultPosition,
			wxDefaultSize,
			wxFileDialogNameStr);

		if (openDialog.ShowModal() != wxID_CANCEL)
		{
			SetImageLocation(ToAString(openDialog.GetPath()));
		}
	}

	void Viewer::OnLoadMessage(Img::MessageReceiver::LoadMessage msg, Img::Image* pImage, const std::string& desc) {
		AddNotification({pImage, msg, desc});
	}

	void Viewer::OnFolderEvent(wxCommandEvent& evt) {
		std::unique_lock<std::mutex> l(m_mutexNotification);
		if (m_folderNotifications.empty()) {
			DO_THROW(Err::CriticalError, "Notification queue is empty.");
		}

		IO::FileEvent notification = m_folderNotifications.front();
		m_folderNotifications.pop_front();
		l.unlock();

		auto full_path = notification.Path + notification.Entry.Name;

		switch (notification.Type) {
			case IO::MonitoredFolderDeleted:
				ActiveImage(Img::Image::Ptr());
				m_cacher.Clear();
				m_folderMonitor.Close();
				break;
			case IO::FileRenamed:
				m_cacher.RenamedImage(notification.Path + notification.Entry.PreviousName, notification.Path + notification.Entry.Name);
				break;
			case IO::FileAdded:
				ActiveImage(m_cacher.AddImageLast(full_path));
				break;
			case IO::FileRemoved:
				ActiveImage(m_cacher.RemoveImageFilename(full_path));
				break;
		}
		UpdateImageInformation();
	}

	void Viewer::AddNotification(const CacheNotification& notification) {
		std::unique_lock<std::mutex> l(m_mutexNotification);
		m_cacheNotifications.push_back(notification);
		l.unlock();

		QueueEvent(new wxCommandEvent(ImageLoadEvent));
	}

	void Viewer::AddNotification(const IO::FileEvent& notification) {
		std::unique_lock<std::mutex> l(m_mutexNotification);
		m_folderNotifications.push_back(notification);
		l.unlock();

		QueueEvent(new wxCommandEvent(FolderEvent));
	}

	void Viewer::UpdateMemoryLimits() {
		if (m_cfg.Cache.DoAutoMemoryLimit)
			m_cacher.MemoryLimit(Sys::Info::TotalPhysicalMemory() / 16);
		else
			m_cacher.MemoryLimit(m_cfg.Cache.ManualMemoryLimit * 1024 * 1024);
	}

	void Viewer::ImageChanged() {
		UpdateImageInformation();

		if (IsMaximized() || m_viewPort.Image() == nullptr)
		{
			return;
		}

		auto imageSize = m_viewPort.OptimalViewportSize();

		if (IsPositive(imageSize) && ViewportMode() == SM_Normal) {
			if (m_cfg.View.ResizeWindow) {
				// The window should be resized some way.
				SizeInt newSize;
				SizeInt windowEdges = wxToSize(GetSize()) - ClientRect().Dimensions();

				if (m_viewPort.ZoomMode() == App::ZoomFitImage) {
					// The image should _also_ be resized to fit some way.
					// Make the viewer as large as possible (and needed).
					//const RectInt& rtDesktop = Win::FindMonitorAt(PositionScreen())->WorkArea();
					wxDisplay display(DisplayFromPointFallback(PositionScreen()));
					auto rtDesktop = wxToRect(display.GetClientArea());

					float xratio = static_cast<float>(rtDesktop.Width() - windowEdges.Width) / imageSize.Width;
					float yratio = static_cast<float>(rtDesktop.Height() - windowEdges.Height) / imageSize.Height;

					ResizeBehaviour mode = m_cfg.View.ResizeBehaviour;
					newSize = calculateImageSize(mode, xratio, yratio, imageSize, windowEdges);
				}
				else {
					newSize = calculateCappedImageSize(m_viewPort.ZoomedImageSize(), windowEdges);
				}

				PointInt newTopLeft = calculateWindowTopLeft(m_cfg.View.ResizePositionMethod, newSize);

				m_userInitiatedMove = false;
				SetSize(newTopLeft.X, newTopLeft.Y, newSize.Width, newSize.Height);
				m_userInitiatedMove = true;
			}
		}
	}

	App::PointInt Viewer::calculateWindowTopLeft(ResizePositionMethod method, const SizeInt &newSize ) {
		wxDisplay display(DisplayFromPointFallback(PositionScreen()));
		auto rtDesktop = wxToRect(display.GetClientArea());

		switch (method) {
		case ResizePositionMethod::PositionToScreen:
			return rtDesktop.TopLeft() + RoundCast((rtDesktop.Dimensions() - newSize) * 0.5f);

		case ResizePositionMethod::PositionToCurrent:
			{
				auto pt = AnchorCenter() + RoundCast(newSize * -0.5f);
				if ((pt.X + newSize.Width) > rtDesktop.Right()) {
					pt.X = rtDesktop.Right() - newSize.Width;
				}
				else if (pt.X < rtDesktop.Left()) {
					pt.X = rtDesktop.Left();
				}

				if (pt.Y + newSize.Height > rtDesktop.Bottom()) {
					pt.Y = rtDesktop.Bottom() - newSize.Height;
				}
				else if (pt.Y < rtDesktop.Top()) {
					pt.Y = rtDesktop.Top();
				}

				AnchorTL(pt);

				return pt;
			}

		case ResizePositionMethod::PositionNothing:
			{
				// Cap to screen
				PointInt pt = AnchorTL();

				if ((pt.X + newSize.Width) > rtDesktop.Right())
					pt.X=rtDesktop.Right()-newSize.Width;
				else if (pt.X < rtDesktop.Left())
					pt.X = rtDesktop.Left();

				if ((pt.Y + newSize.Height) > rtDesktop.Bottom())
					pt.Y=rtDesktop.Bottom()-newSize.Height;
				else if (pt.Y < rtDesktop.Top())
					pt.Y=rtDesktop.Top();

				AnchorCenter(pt + RoundCast(newSize * 0.5f));

				return pt;
			}

		default:
			DO_THROW(Err::InvalidParam, "Invalid reposition method: " + ToAString(method));
		}
	}

	SizeInt Viewer::calculateImageSize( ResizeBehaviour mode, float xratio, float yratio, const SizeInt &imageSize, const SizeInt &windowEdges ) {
		wxDisplay display(DisplayFromPointFallback(PositionScreen()));
		auto rtDesktop = wxToRect(display.GetClientArea());

		// The image is larger than the desktop. The image is not supposed
		// to be downscaled so fill the screen.
		if ((mode == ResizeEnlargeOnly) && (xratio < 1.0) && (yratio < 1.0))
			return rtDesktop.Dimensions();
		// The image is smaller than the desktop. It must not be made
		// smaller needlessly so size the window after the image.
		else if ((mode == ResizeReduceOnly) && (xratio > 1.0) && (yratio > 1.0))
			return SizeInt(
				std::max<int>(MinWindowWidth, imageSize.Width + windowEdges.Width),
				std::max<int>(MinWindowHeight,imageSize.Height + windowEdges.Height));

		return Maximum(SizeInt(MinWindowWidth, MinWindowHeight), Minimum(RoundCast(imageSize * std::min(xratio, yratio)) + windowEdges, rtDesktop.Dimensions()));
	}

	SizeInt Viewer::calculateCappedImageSize( const SizeInt& imageSize, const SizeInt &windowEdges ) {
		wxDisplay display(DisplayFromPointFallback(PositionScreen()));
		auto rtDesktop = wxToRect(display.GetClientArea());

		return SizeInt(
			std::max<int>(MinWindowWidth,  std::min<int>(rtDesktop.Width(), imageSize.Width + windowEdges.Width)),
			std::max<int>(MinWindowHeight,	std::min<int>(rtDesktop.Height(),imageSize.Height + windowEdges.Height)));
	}

	std::string Viewer::UII_LastModified(std::time_t date) {
		std::stringstream ss;
		if (date != 0)
		{
			typedef boost::date_time::c_local_adjustor<boost::posix_time::ptime> local_adj;
			auto pt = boost::posix_time::from_time_t(date);
			auto localdate = local_adj::utc_to_local(pt);

			auto* f = new boost::posix_time::time_facet("%Y-%m-%d %H:%M");
			ss.imbue(std::locale(std::locale(), f));
			ss << localdate;
		}
		return ss.str();
	}

	void Viewer::AdjustChange(int brightness, int contrast, int gamma) {
		m_viewPort.Brightness(brightness);
		m_viewPort.Contrast(contrast);
		m_viewPort.Gamma(gamma);
		m_viewPort.Refresh();
	}

	void Viewer::ShowAdjust() {
		m_adjust->Show();
		m_adjust->Raise();
		m_adjust->Brightness(m_viewPort.Brightness());
		m_adjust->Contrast(m_viewPort.Contrast());
		m_adjust->Gamma(m_viewPort.Gamma());
	}

	void Viewer::SetImageLocation(std::string path) {
		auto fixedPath = boost::filesystem::path(path);
		if (IO::DoPathExist(path))
		{
			fixedPath += boost::filesystem::path::preferred_separator;
		}
		else if (IO::DoFileExist(fixedPath.string()) == false)
		{
			return;
		}

		m_folderMonitor.Close();
		if (m_folder.Path(IO::GetPath(fixedPath.string())) == false)
		{
			return;
		}

		m_folderMonitor.Start(m_folder.Path());

		Img::FillCacher(m_cacher, m_folder);

		size_t imageIndex = 0;
		if (m_cacher.FindImage(path, &imageIndex))
		{
			ActiveImage(m_cacher.GotoImage(imageIndex));
		}
		else
		{
			ActiveImage(m_cacher.FirstImage());
		}
	}

	void Viewer::ShowContextMenu(Win::MouseEvent e) {
		// Disable automatic hiding while the context menu is visible
		ViewPort::CursorMode m=m_viewPort.ActiveCursorMode();
		m_viewPort.ActiveCursorMode(ViewPort::CursorShow);
		PopupMenu(&m_contextMenu, e.Position.X, e.Position.Y);

		// Return to whatever it was originally
		m_viewPort.ActiveCursorMode(m);
	}

	void Viewer::ShowSettings()
	{
		m_settings->Show(true);
		m_settings->Raise();
	}

	void Viewer::OpenDirectoryInExplorer() {
		auto pImage = m_viewPort.Image();
		if (pImage == nullptr)
		{
			return;
		}

		ShowInFileManager(m_cacher.CurrentImageFilename());
	}

	void Viewer::Sort(Img::Cacher::SortMethod m) {
		ActiveImage(m_cacher.Sort(m));
		UpdateImageInformation();
	}

	void Viewer::SettingsChanged() {
		Reg::Save(cg_SettingsLocation, m_cfg);
		m_cacher.WrapAround(m_cfg.View.BrowseWrapAround);
		m_keys.SetBindings(m_cfg.Keyboard);

		m_statusBar->Show(m_cfg.View.ShowStatusBar && ViewportMode() != SM_Fullscreen);

		UpdateViewportConfig();

		UpdateMemoryLimits();
		m_viewPort.Refresh();

		AlwaysOnTop(m_cfg.View.AlwaysOnTop);
		ImageChanged();
	}

	void Viewer::UpdateViewportConfig()
	{
		m_viewPort.MinificationFilter(m_cfg.Render.MinFilter);
		m_viewPort.MagnificationFilter(m_cfg.Render.MagFilter);
		m_viewPort.ResizeBehaviour(m_cfg.View.ResizeBehaviour);
		m_viewPort.ResetPan(m_cfg.View.ResetPan);
		m_viewPort.MouseConfig(m_cfg.Mouse);
		m_viewPort.BackgroundColor(m_cfg.Render.BackgroundColor);
	}

	Geom::PointInt Viewer::PositionScreen()
	{
		return Win::wxToPoint(GetPosition());
	}

	Geom::PointInt Viewer::CenterPositionScreen()
	{
		auto rect = wxToRect(GetScreenRect());
		return { rect.Left() + rect.Width() / 2, rect.Top() + rect.Height() / 2 };
	}

	Geom::RectInt Viewer::ClientRect()
	{
		auto rect = GetClientRect();
		return Geom::Rect<int>(Geom::PointInt(rect.x, rect.y), Geom::SizeInt(rect.width, rect.height));
	}

	void Viewer::AlwaysOnTop(bool doAlwaysOnTop)
	{
		auto style = GetWindowStyleFlag();
		if (doAlwaysOnTop)
		{
			style |= wxSTAY_ON_TOP;
		}
		else
		{
			style &= ~(wxSTAY_ON_TOP);
		}
		SetWindowStyleFlag(style);
	}

	unsigned int Viewer::DisplayFromPointFallback(Geom::PointInt position)
	{
		auto displayIndex = wxDisplay::GetFromPoint(PointToWx(position));
		if(displayIndex < 0)
		{
			// We didn't find a display, but crashing here would be a bit frustrating for the user.
			// We'll pick the first display and use that instead, which might cause the window to jump
			// a bit but that should still be less frustrating.
			displayIndex = 0;
		}
		return displayIndex;
	}

	Geom::SizeInt Viewer::GetVisibleImageSize() const
	{
		return m_viewPort.GetVisibleImageSize();
	}
}
