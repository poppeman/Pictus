#include "StdAfx.h"

#include "orz/file.h"
#include "orz/stream_file.h"
#include "orz/fileops.h"
#include "orz/intl.h"
#include "orz/orz_math.h"
#include "orz/sysinfo.h"

#include "imagecache.h"
#include "illa/surfacemgr.h"

#include "rename.h"

#include "gdiwrapper.h"
#include "registry.h"

#include "getevent.h"
#include "viewer.h"

#include "config.h"
#include "res_viewer.h"

#include "win7taskbar.h"

#include "filterstring.h"
#include "builder_viewport.h"
#include "timeconvert.h"

const wchar_t* App::Viewer::ClassName = TX("Pictus Viewer");
const wchar_t* App::Viewer::AppTitle = TX("Pictus");

namespace App {
	using namespace Win;
	using namespace Intl;
	using namespace Reg::Keys;

	using namespace Geom;

	void ValidateFilterMode(Reg::Keys::DWORDKey dw) {
		if (Reg::Key(dw) >= static_cast<int>(Filter::Mode::Num)) {
			Reg::Key(dw, static_cast<int>(Filter::Mode::Bilinear));
		}
	}

	Viewer::Viewer(Img::CodecFactoryStore* cfs, const std::wstring params)
		:m_singleMutex{ 0 },
		m_doMaximize{ false },
		m_screenMode{ SM_Normal },
		m_codecs{ cfs },
		m_attemptToShow{ false }
	{
		m_adjust.OnChange.connect([this](int a, int b, int c) { AdjustChange(a, b, c); });
		OnMouseButtonDown.connect([this](Win::MouseEvent e) { return m_mouseMap.Execute(MouseStandardEvent(e), e); });
		OnMouseWheel.connect([this](Win::MouseEvent e) { return m_mouseMap.Execute(MouseStandardEvent(e), e); });
		OnMouseButtonDoubleClick.connect([this](Win::MouseEvent e) { return m_mouseMap.Execute(MouseDblEvent(e), e); });
		OnTaskbarButton.connect([this](int id) { PerformOnTaskbarButton(id); });

		m_mouseMap.AddAction(MouseFullscreen, [this](Win::MouseEvent) { ToggleFullscreenMode(); });
		m_mouseMap.AddAction(MouseToggleFullSizeDefaultZoom, [&](Win::MouseEvent) { ZoomToggleFullSizeDefaultZoom(); });
		m_mouseMap.AddAction(MouseNextImage, [this](Win::MouseEvent) { ImageNext(1); });
		m_mouseMap.AddAction(MousePrevImage, [this](Win::MouseEvent) { ImagePrev(1); });
		m_mouseMap.AddAction(MouseZoomIn, [this](Win::MouseEvent) { ZoomIn(); });
		m_mouseMap.AddAction(MouseZoomOut, [this](Win::MouseEvent) { ZoomOut(); });
		m_mouseMap.AddAction(MouseContext, [this](Win::MouseEvent e) { ShowContextMenu(e); });

		m_contextMenu.Construct(this);
		m_keys.Construct(this);

		m_lang = Intl::OnLanguageChanged.connect([&]() { UpdateImageInformation(); });

		m_settings = std::make_shared<Settings>(m_codecs);
		m_settings->OnSettingsChanged.connect([&]() { SettingsChanged(); });

		m_cacher.SetCodecFactoryStore(m_codecs);

		m_shellTaskbarImageList.Load(IDB_THUMBTOOLBAR,
			16,
			0,
			CLR_DEFAULT);

		ValidateFilterMode(DWMagFilter);
		ValidateFilterMode(DWMinFilter);

		if ((params.length() > 0) && (params.at(0) != TX('-')) && (IO::DoFileExist(params) || IO::DoPathExist(params))) {
			m_sDirectory = params;
		}
		else {
			m_sDirectory = L"";
		}

		// Look for another process (disallow if the setting requires that)
		m_singleMutex = CreateMutex(0, true, (std::wstring(TX("Local\\")) + ClassName).c_str());

		if ((GetLastError() == ERROR_ALREADY_EXISTS) && (Reg::Key(DWMultipleInstances) == false)) {
			if (HWND hwnd = ::FindWindow(ClassName, 0)) {
				SetForegroundWindow(hwnd);
				if (IsIconic(hwnd)) {
					ShowWindow(hwnd, SW_RESTORE);
				}

				// Tell the instance to open the new location/file (if applicable)
				if (params.empty() == false) {
					// Dodge const-incorrectness
					boost::scoped_array<wchar_t> pStrData(new wchar_t[params.length() + 1]);
					wcscpy_s(pStrData.get(), (params.length() + 1), params.c_str());
					COPYDATASTRUCT cds;
					cds.dwData = 0;
					cds.cbData = static_cast<DWORD>((params.length() + 1) * sizeof(wchar_t));
					cds.lpData = reinterpret_cast<void*>(pStrData.get());
					SendMessage(hwnd, WM_COPYDATA, (WPARAM)Handle(), (LPARAM)&cds);
				}

				throw Err::DuplicateInstance();
			}
		}
	}

	Viewer::~Viewer() {
		m_folderMonitor.Close();
		m_cacher.Stop();

		UnregisterClass(ClassName, GetModuleHandle(0));

		ReleaseMutex(m_singleMutex);
		CloseHandle(m_singleMutex);

		if (UnhandledExceptionOcurred()) {
			MessageBox(nullptr, UnhandledExceptionDescription().c_str(), L"Pictus error", MB_OK);
		}
	}

	bool Viewer::Show(bool doShow) {
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
	}

	void Viewer::ActiveImage(Img::Image::Ptr pImage) {
		if (m_viewPort.Image() == pImage) {
			return;
		}

		m_viewPort.Image(pImage);

		if (Reg::Key(DWResetZoom)) {
			ZoomMode(App::ZoomMode(Reg::Key(DWDefaultZoomMode)));
		}

		UpdateImageInformation();
		ImageChanged();
	}

	bool Viewer::PerformOnCreate() {
		// Apply some settings that can't be set automatically
		UpdateMemoryLimits();

		m_cacher.MessageTarget(this);
		// TODO: FIXME: This is very wrong, should be set to something that isn't hardcoded.
		m_cacher.SetMaximumResolutionHint(Geom::SizeInt(64, 64));
		m_folderMonitor.OnEvent.connect([this](IO::FileEvent e) { FolderEvent(e); });

		m_cacher.WrapAround(Reg::Key(DWBrowseWrapAround) != 0);

		WNDCLASSEX wc;
		ZeroMemory(&wc, sizeof(wc));
		wc.cbSize			= sizeof(wc);
		wc.hCursor			= LoadCursor(0, IDC_ARROW);
		wc.hIcon			= LoadIcon(GetHINSTANCE(), MAKEINTRESOURCE(IDI_APP_MAIN));
		wc.hInstance		= GetHINSTANCE();
		wc.lpfnWndProc		= WndProcDelegate;
		wc.lpszClassName	= ClassName;
		wc.style			= CS_DBLCLKS;
		RegisterClassEx(&wc);

		int w = Util::Constrain<int>(MinWindowWidth, Reg::Key(DWWindowSizeWidth), GetSystemMetrics(SM_CXVIRTUALSCREEN));
		int h = Util::Constrain<int>(MinWindowHeight, Reg::Key(DWWindowSizeHeight), GetSystemMetrics(SM_CYVIRTUALSCREEN));

		int x = Util::Constrain<int>(0, Reg::Key(DWWindowPosX), GetSystemMetrics(SM_CXVIRTUALSCREEN) - w);
		int y = Util::Constrain<int>(0, Reg::Key(DWWindowPosY), GetSystemMetrics(SM_CYVIRTUALSCREEN) - h);

		if(!ConstructWindow(RectInt(PointInt(x, y), SizeInt(w, h)), WS_EX_ACCEPTFILES, ClassName, AppTitle, WS_OVERLAPPEDWINDOW)) return false;
		m_previousWindowStyle = GetWindowLongPtr(Handle(), GWL_STYLE);

		AlwaysOnTop(Reg::Key(DWAlwaysOnTop) != 0);

		ZoomMode(App::ZoomMode(Reg::Key(DWDefaultZoomMode)));

		if (Reg::Key(DWMaximized)) {
			m_doMaximize = true;
		}

		return true;
	}

	bool Viewer::PerformOnWindowCreate() {
		m_statusBar = std::make_shared<Win::StatusBar>();
		Add(m_statusBar);
		m_statusParts[StatusZoom].Width(StatFieldZoomWidth);
		m_statusParts[StatusName].AutoSize(Win::StatusBarPart::AutoSizeShare);
		m_statusParts[StatusResolution].Width(StatFieldImageDimWidth);
		m_statusParts[StatusPosition].Width(StatFieldPosWidth);
		m_statusParts[StatusProgress].Width(StatFieldTimeWidth);
		m_statusParts[StatusFileSize].Width(StatFieldFileSizeWidth);
		m_statusParts[StatusLastModified].Width(StatFieldLastModified);
		for(int i=0;i<StatusNumParts; ++i)
			m_statusBar->Add(m_statusParts[i]);

		m_statusBar->Visible(Reg::Key(DWShowStatusBar)!=0);

		try {
			ViewportBuilder b;
			b.BuildViewport(m_viewPort, this);
		}
		catch(Err::Exception& e) {
			MessageBox(0, (GetWString(SIDDirectXFailed) + std::wstring(L"\n\n") + e.Desc()).c_str(), 0, MB_OK);
			return false;
		}

		if ((m_sDirectory.length() > 0) && (m_sDirectory.at(0) != TX('-')))
			SetImageLocation(m_sDirectory);

		return true;
	}

	bool Viewer::PerformOnClose() {
		SizeInt sz	= GetSize();
		PointInt pt	= PositionScreen();

		if(m_screenMode == SM_Fullscreen) {
			sz = m_previousNonMaximizedWindowRegion.Dimensions();
			pt = m_previousNonMaximizedWindowRegion.TopLeft();
		}

		// Store un-maximized values instead!
		WINDOWPLACEMENT wp = { 0 };
		GetWindowPlacement(Handle(), &wp);
		if (IsZoomed(Handle())) {
			RectInt normalRect(RECTToRect(wp.rcNormalPosition));
			sz = normalRect.Dimensions();
			pt = normalRect.TopLeft();
		}

		// Don't want to save bogus values!
		if (!IsIconic(Handle())) {
			Reg::Key(DWWindowSizeWidth, sz.Width);
			Reg::Key(DWWindowSizeHeight, sz.Height);
			Reg::Key(DWWindowPosX, pt.X);
			Reg::Key(DWWindowPosY, pt.Y);
		};

		Reg::Key(DWMaximized, IsZoomed(Handle()));

		return true;
	}

	bool Viewer::PerformOnchar(uint32_t key, int , bool ) {
		switch (key) {
			// Copy
			case 3:
			{
				Img::Image::Ptr pImg = m_viewPort.Image();
				if (!pImg.get()) return false;
				if (pImg->IsFinished() == false) return false;

				Geom::SizeInt size(pImg->GetSize());
				Img::Surface::Ptr surface = pImg->CurrentSurface();
				if (surface == 0)
					return false;

				// Signal an error here
				if (!OpenClipboard(Handle()) || !EmptyClipboard())
					return false;

				HDC hDC = GetDC(Handle());

				HBITMAP hBitmap = CreateCompatibleBitmap(hDC, size.Width, size.Height);

				Win::SharedDC dc;
				dc.SelectObject(hBitmap);

				boost::scoped_array<uint8_t> pBuf(new uint8_t[size.Width * 4]);

				BITMAPINFO bmi;
				bmi.bmiHeader.biSize		= sizeof(bmi.bmiHeader);
				bmi.bmiHeader.biWidth		= size.Width;
				bmi.bmiHeader.biHeight		= size.Height;
				bmi.bmiHeader.biBitCount	= 4 * 8;
				bmi.bmiHeader.biCompression	= BI_RGB;
				bmi.bmiHeader.biPlanes		= 1;

				for (int i = 0; i < (size.Height); ++i) {
					surface->CopyRectToBuffer(
						pBuf.get(),
						size.Width * 4,
						RectInt(PointInt(0, i), SizeInt(size.Width, 1)),
						m_viewPort.BackgroundColor(),
						m_viewPort.Brightness(),
						m_viewPort.Contrast(),
						m_viewPort.Gamma(), false);
					int copied = SetDIBitsToDevice(dc, 0, i, size.Width, 1, 0, 0, 0, 1, pBuf.get(), &bmi, 0);
					copied = copied;
				}

				HANDLE hClipData = SetClipboardData(CF_BITMAP, hBitmap);
				DeleteObject(hBitmap);

				// Signal an error here
				if (!CloseClipboard() || !hClipData)
					return false;

				ReleaseDC(Handle(), hDC);

				break;
			}

		case 22:
			// Paste
			break;
		default:
			return false;
		}
		return true;
	}

	bool Viewer::PerformOnSize(const SizeInt&) {
		if(ViewportMode() == SM_Fullscreen)
			return false;

		if(!IsZoomed(Handle()) && !IsIconic(Handle()))
			m_previousNonMaximizedWindowRegion = WindowRect();

		RectInt client = ClientRect();

		if(m_statusBar->Visible()) {
			m_viewPort.Resize(SizeInt(client.Width(), client.Height() - m_statusBar->Position().Height()));
			UpdateImageInformation();
		}
		else
			m_viewPort.Resize(client.Dimensions());

		return true;
	}

	bool Viewer::PerformOnMove(const PointInt&, bool byUser) {
		if (!byUser) {
			return false;
		}

		AnchorTL(PositionScreen());
		AnchorCenter(PositionScreen() + RoundCast(GetSize() * 0.5f));

		return true;
	}

	void Viewer::AnchorTL(const PointInt& pt) {
		Reg::Key(DWWindowAnchorTLX, pt.X);
		Reg::Key(DWWindowAnchorTLY, pt.Y);
	}

	PointInt Viewer::AnchorTL() const {
		return PointInt(Reg::Key(DWWindowAnchorTLX), Reg::Key(DWWindowAnchorTLY));
	}

	void Viewer::AnchorCenter(const PointInt& pt) {
		Reg::Key(DWWindowAnchorCenterX, pt.X);
		Reg::Key(DWWindowAnchorCenterY, pt.Y);
	}

	PointInt Viewer::AnchorCenter() const {
		return PointInt(Reg::Key(DWWindowAnchorCenterX), Reg::Key(DWWindowAnchorCenterY));
	}

	bool Viewer::PerformOnDropFiles(const StringVector& files) {
		if(files.size() == 0) return false;

		SetImageLocation(files[0]);
		return true;
	}

	bool Viewer::PerformOnApp(int index, WPARAM, LPARAM) {
		switch(index) {
			case ViewerImageLoadEvent:
				HandleCacheNotification();
				return true;
			case ViewerListUpdate:
				HandleFolderNotification();
				return true;
		}
		return false;
	}

	bool Viewer::PerformOnCopyData(const COPYDATASTRUCT* pcds) {
		if (pcds->dwData != 0) return false;

		SetImageLocation(std::wstring(reinterpret_cast<wchar_t*>(pcds->lpData)));
		return true;
	}

	bool Viewer::ApplyWallpaper(Win::Wallpaper::Mode orientation) {
		Img::Image::Ptr pImage(m_viewPort.Image());
		if (pImage == 0) {
			return false;
		}

		Wallpaper p;
		return p.ApplyWallpaper(m_cacher.CurrentImageFilename(), orientation);
	}

	void Viewer::UpdateImageInformation() {
		if (!m_statusBar) {
			return;
		}

		if (ViewportMode() == SM_Fullscreen) {
			return m_statusBar->Visible(false);
		}

		Img::Image::Ptr image(m_viewPort.Image());
		m_statusParts[StatusProgress].Text(UII_LoadProgress(image));
		m_statusParts[StatusResolution].Text(UII_ImageResolution(image));

		Caption((image ? 
			IO::GetFile(m_cacher.CurrentImageFilename()) + TX(" - "):
			TX("")) + std::wstring(AppTitle));

		if (image == 0) {
			for (int i = 0; i < StatusNumParts; ++i) {
				m_statusParts[i].Text(TX(""));
			}
		}
		else {
			m_statusParts[StatusName].Text(IO::GetFile(m_cacher.CurrentImageFilename()));
			m_statusParts[StatusPosition].Text(ToWString(m_cacher.CurrentImageIndex() + 1) + TX("\\") + ToWString(m_cacher.ImageCount()));
			m_statusParts[StatusFileSize].Text(UII_MemoryUsage(m_cacher.CurrentImageFileSize()));

			if (image->IsHeaderInformationValid() == false && image->IsFinished()) {
				m_statusParts[StatusZoom].Text(TX(""));
			}
			else {
				m_statusParts[StatusZoom].Text(ToWString(RoundCast(m_viewPort.ZoomLevel() * 100.0f)) + TX("%"));
				m_statusParts[StatusLastModified].Text(UII_LastModified(m_cacher.CurrentImageLastModifiedDate()));
			}
		}
	}

	std::wstring Viewer::UII_MemoryUsage(FileInt size) {
		FileInt alloc = size * FileSizeDivider;
		FileInt kbSize = alloc >> 10;
		FileInt mbSize = kbSize >> 10;
		FileInt gbSize = mbSize >> 10;

		if (gbSize > FileSizeDivider) {
			return ToWString(gbSize / (float)FileSizeDivider) + TX(" ") + GetWString(SIDGBytes);
		}
		else if (mbSize > FileSizeDivider) {
			return ToWString(mbSize / (float)FileSizeDivider) + TX(" ") + GetWString(SIDMBytes);
		}
		else if (kbSize > FileSizeDivider) {
			return ToWString(kbSize / (float)FileSizeDivider) + TX(" ") + GetWString(SIDKBytes);
		}
		else {
			return ToWString(alloc / (float)FileSizeDivider) + TX(" ") + GetWString(SIDBytes);
		}
	}

	std::wstring Viewer::UII_LoadProgress(Img::Image::Ptr image) {
		if (image == nullptr || (image->IsHeaderInformationValid() == false && image->IsFinished())) {
			return L"";
		}

		if (image->IsFinished()) {
			return GetWString(SIDLoadedIn) + ToWString(image->LoadTime() / 1000.0f) + GetWString(SIDSeconds);
		}

		return GetWString(SIDLoading);
	}

	std::wstring Viewer::UII_ImageResolution(Img::Image::Ptr image) {
		if(!image || (image->IsHeaderInformationValid() == false && image->IsFinished()))
			return TX("");

		SizeInt sz = image->GetSize();
		if(IsPositive(sz))
			return ToWString(sz.Width) + TX("x") + ToWString(sz.Height);

		return TX("");
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
		if (m_viewPort.ZoomMode() == App::ZoomFullSize) {
			ZoomMode(App::ZoomMode(Reg::Key(DWDefaultZoomMode)));
		}
		else {
			ZoomFullSize();
		}
	}

	void Viewer::ZoomFullSize() {
		m_viewPort.ZoomMode(App::ZoomFullSize);
		m_contextMenu.Zoomed(true);
		ImageChanged();
		UpdateImageInformation();
	}

	void Viewer::ZoomMode(App::ZoomMode mode) {
		m_viewPort.ZoomMode(mode);
		switch (mode) {
			case App::ZoomFitImage:
				m_contextMenu.FitImage();
				break;

			case App::ZoomFree:
				m_contextMenu.Zoomed(true);
				break;
		}

		ImageChanged();
		UpdateImageInformation();
	}

	void Viewer::ToggleFullscreenMode() {
		if (m_screenMode == Viewer::SM_Normal)
			ViewportMode(Viewer::SM_Fullscreen);
		else
			ViewportMode(Viewer::SM_Normal);
	}

	void Viewer::ViewportMode(ScreenMode newMode) {
		m_screenMode = newMode;

		if (newMode == Viewer::SM_Fullscreen) {
			const Win::Monitor* mon = Win::FindMonitorAt(CenterPositionScreen());

			m_statusBar->Visible(false);
			m_previousWindowStyle = GetWindowLongPtr(Handle(), GWL_STYLE);
			SetWindowLongPtr(Handle(), GWL_STYLE, WS_POPUP | WS_VISIBLE | WS_CLIPCHILDREN);
			m_previousWindowRegion = WindowRect();
			MoveResize(mon->Region());
			m_viewPort.MoveResize(RectInt(PointInt(0, 0), mon->Region().Dimensions()));
			m_viewPort.ActiveCursorMode(ViewPort::CursorHideAutomatic);
			SetForegroundWindow(Handle());
		}
		else {
			SetWindowLongPtr(Handle(), GWL_STYLE, m_previousWindowStyle);
			m_statusBar->Visible(Reg::Key(DWShowStatusBar) != 0);
			MoveResize(m_previousWindowRegion);
			m_viewPort.ActiveCursorMode(ViewPort::CursorShow);
		}

		// Make sure that the settings and adjust dialogs are on top (if running)
		if (m_settings->IsModelessVisible()) ShowSettings();
		if (m_adjust.IsModelessVisible()) ShowAdjust();

		ImageChanged();
		UpdateImageInformation();
	}

	Viewer::ScreenMode Viewer::ViewportMode() const {
		return m_screenMode;
	}

	void Viewer::PanUp() {
		m_viewPort.Pan(SizeInt(0, -KeyPanSize));
	}
	void Viewer::PanRight() {
		m_viewPort.Pan(SizeInt(KeyPanSize, 0));
	}
	void Viewer::PanDown() {
		m_viewPort.Pan(SizeInt(0, KeyPanSize));
	}
	void Viewer::PanLeft() {
		m_viewPort.Pan(SizeInt(-KeyPanSize, 0));
	}

	void Viewer::Rotate(Filter::RotationAngle r) {
		m_viewPort.Rotate(r);

		UpdateImageInformation();
		ImageChanged();
		m_viewPort.ImageUpdated();
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

		Math::Randomizer random;
		size_t pos = random.Random() % (imageCount - 1);

		if (pos >= m_cacher.CurrentImageIndex()) pos++;

		ActiveImage(m_cacher.GotoImage(pos));
	}

	void Viewer::ToEnd() {
		ActiveImage(m_cacher.LastImage());
	}

	void Viewer::RenameCurrent() {
		Img::Image::Ptr image(m_viewPort.Image());
		if (image == 0) return;

		std::wstring old_name(m_cacher.CurrentImageFilename());
		std::wstring extension = IO::GetExtension(old_name);

		Rename ren(IO::GetTitle(old_name));

		if (ren.DoModal(this)) {
			std::wstring new_name = IO::GetPath(old_name) + ren.Name() + TX(".") + extension;
			IO::FileReader::Ptr reader = m_cacher.CurrentImageFileReader();
			if(!reader)
				return;
			std::wstring resulting_name = dynamic_cast<IO::StreamFile*>(reader->GetStream())->Rename(new_name, Handle());
			m_cacher.RenamedImage(old_name, resulting_name);

			UpdateImageInformation();
		}
	}


	void Viewer::RemoveImage(RemoveOp op) {
		Img::Image::Ptr image(m_viewPort.Image());
		if (image == 0) return;

		if (op == RemoveOnly) {
			ActiveImage(m_cacher.RemoveCurrentImage());
			return;
		}

		std::wstring old_name(m_cacher.CurrentImageFilename());

		IO::FileReader::Ptr reader = m_cacher.CurrentImageFileReader();
		if(!reader)
			return;

		IO::StreamFile* f = dynamic_cast<IO::StreamFile*>(reader->GetStream());
		if(f->Delete((op == RemoveRecycle), Handle()))
			ActiveImage(m_cacher.RemoveCurrentImage());

		UpdateImageInformation();
	}

	void Viewer::OpenFolder() {
		FilterString s(m_codecs);
		std::wstring file = OpenFileDialog(GetWString(SIDOpenFile), s.GetFilterString().c_str(), s.FilterCount());
		if (file != TX(""))
			SetImageLocation(file);
	}

	void Viewer::OnLoadMessage(Img::MessageReceiver::LoadMessage msg, Img::Image* pImage, const std::wstring& desc) {
		CacheNotification not = {pImage, msg, desc};
		AddNotification(not);
	}

	void Viewer::HandleCacheNotification() {
		std::unique_lock<std::mutex> l(m_mutexNotification);
		if (m_cacheNotifications.empty()) {
			DO_THROW(Err::CriticalError, TX("Notification queue is empty."));
		}

		CacheNotification not = m_cacheNotifications.front();
		m_cacheNotifications.pop_front();
		l.unlock();

		if(not.message == Img::MessageReceiver::LoadErrorCritical) {
			m_exceptionDescription = not.desc;
			m_exceptionOcurred = true;
			PostMessage(Handle(), WM_CLOSE, 0, 0);
		}
		if (not.image == m_viewPort.Image().get()) {
			switch (not.message) {
				case Img::MessageReceiver::LoadDone:
					m_viewPort.ImageUpdated();
					UpdateImageInformation();
					m_viewPort.Redraw();
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

			if(m_attemptToShow && not.message >= Img::MessageReceiver::LoadAllocated) Show(true);
		}
	}

	void Viewer::HandleFolderNotification() {
		std::unique_lock<std::mutex> l(m_mutexNotification);
		if (m_folderNotifications.empty()) {
			DO_THROW(Err::CriticalError, TX("Notification queue is empty."));
		}

		IO::FileEvent not = m_folderNotifications.front();
		m_folderNotifications.pop_front();
		l.unlock();

		std::wstring full_path = not.Path + not.Entry.Name;

		switch (not.Type) {
			case IO::MonitoredFolderDeleted:
				ActiveImage(Img::Image::Ptr());
				m_cacher.Clear();
				m_folderMonitor.Close();
				break;
			case IO::FileRenamed:
				m_cacher.RenamedImage(not.Path + not.Entry.PreviousName, not.Path + not.Entry.Name);
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

		QueueMessage(WM_APP + ViewerImageLoadEvent, 0, 0);
	}

	void Viewer::AddNotification(const IO::FileEvent& notification) {
		std::unique_lock<std::mutex> l(m_mutexNotification);
		m_folderNotifications.push_back(notification);
		l.unlock();

		QueueMessage(WM_APP + ViewerListUpdate, 0, 0);
	}

	void Viewer::UpdateMemoryLimits() {
		if (Reg::Key(DWDoAutoMemoryLimit) != 0)
			m_cacher.MemoryLimit(Sys::Info::TotalPhysicalMemory() / 16);
		else
			m_cacher.MemoryLimit(Reg::Key(DWManualMemoryLimit) * 1024 * 1024);
	}

	void Viewer::ImageChanged() {
		if (IsZoomed(Handle()) || m_viewPort.Image() == nullptr) {
			return;
		}

		auto imageSize = m_viewPort.OptimalViewportSize();

		if (!IsPositive(imageSize)) {
			return;
		}

		if (ViewportMode() != SM_Normal) {
			return;
		}

		if (Reg::Key(DWResizeWindow)) {
			// The window should be resized some way.
			SizeInt newSize;
			SizeInt windowEdges = GetSize() - ClientRect().Dimensions();

			if (Reg::Key(DWShowStatusBar)) {
				windowEdges.Height += m_statusBar->Position().Height();
			}

			if (m_viewPort.ZoomMode() == App::ZoomFitImage) {
				// The image should _also_ be resized to fit some way.
				// Make the viewer as large as possible (and needed).
				const RectInt& rtDesktop = Win::FindMonitorAt(PositionScreen())->WorkArea();

				float xratio = static_cast<float>(rtDesktop.Width() - windowEdges.Width) / imageSize.Width;
				float yratio = static_cast<float>(rtDesktop.Height() - windowEdges.Height) / imageSize.Height;

				ResizeBehaviour mode = ResizeBehaviour(Reg::Key(DWResizeBehaviour));
				newSize = calculateImageSize(mode, xratio, yratio, imageSize, windowEdges);
			}
			else {
				newSize = calculateCappedImageSize(m_viewPort.ZoomedImageSize(), windowEdges);
			}

			PointInt newTopLeft = calculateWindowTopLeft(ResizePositionMethod(Reg::Key(DWResizePositionMethod)), newSize);

			MoveResize(newTopLeft, newSize);

		}
		else {
			OnSize(GetSize());	// Adjust to the current window
		}

		UpdateImageInformation();
		//if(m_attemptToShow) Show(m_attemptToShow);
	}

	App::PointInt Viewer::calculateWindowTopLeft(ResizePositionMethod method, const SizeInt &newSize ) {
		const RectInt& rtDesktop	= Win::FindMonitorAt(PositionScreen())->WorkArea();

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
			DO_THROW(Err::InvalidParam, TX("Invalid reposition method: ") + ToWString(method));
		}
	}

	SizeInt Viewer::calculateImageSize( ResizeBehaviour mode, float xratio, float yratio, const SizeInt &imageSize, const SizeInt &windowEdges ) {
		const RectInt& rtDesktop	= Win::FindMonitorAt(PositionScreen())->WorkArea();

		// The image is larger than the desktop. The image is not supposed
		// to be downscaled so fill the screen.
		if ((mode == ResizeEnlargeOnly) && (xratio < 1.0) && (yratio < 1.0))
			return rtDesktop.Dimensions();

		// The image is smaller than the desktop. It must not be made
		// smaller needlessly so size the window after the image.
		else if ((mode == ResizeReduceOnly) && (xratio > 1.0) && (yratio > 1.0))
			return SizeInt(
				Util::Max<int>(MinWindowWidth, imageSize.Width + windowEdges.Width),
				Util::Max<int>(MinWindowHeight,imageSize.Height + windowEdges.Height));

		return Maximum(SizeInt(MinWindowWidth, MinWindowHeight), Minimum(RoundCast(imageSize * Util::Min(xratio, yratio)) + windowEdges, rtDesktop.Dimensions()));
	}

	SizeInt Viewer::calculateCappedImageSize( const SizeInt& imageSize, const SizeInt &windowEdges ) {
		const RectInt& rtDesktop	= Win::FindMonitorAt(PositionScreen())->WorkArea();

		return SizeInt(
			Util::Max<int>(MinWindowWidth,  Util::Min<int>(rtDesktop.Width(), imageSize.Width + windowEdges.Width)),
			Util::Max<int>(MinWindowHeight,	Util::Min<int>(rtDesktop.Height(),imageSize.Height + windowEdges.Height)));
	}

	std::wstring Viewer::UII_LastModified(FileInt date) {
		return FormattedDate(date) + TX(" ") + FormattedTime(date);
	}

	bool Viewer::PerformOnCreateTaskbar() {
		Win::Win7Taskbar taskbar;
		taskbar.AddButton(0, 0);
		taskbar.AddButton(1, 1);
		taskbar.SetImageList(m_shellTaskbarImageList.Handle());
		taskbar.Create(Handle());
		return true;
	}

	bool Viewer::PerformOnTaskbarButton(int id) {
		switch(id) {
			case 0:
				ImagePrev(1);
				break;
			case 1:
				ImageNext(1);
				break;
			default:
				return false;
		}
		return true;
	}

	void Viewer::AdjustChange(int brightness, int contrast, int gamma) {
		m_viewPort.Brightness(brightness);
		m_viewPort.Contrast(contrast);
		m_viewPort.Gamma(gamma);
		m_viewPort.Redraw();
	}

	void Viewer::ShowAdjust() {
		m_adjust.DoModeless(this);
		m_adjust.Brightness(m_viewPort.Brightness());
		m_adjust.Contrast(m_viewPort.Contrast());
		m_adjust.Gamma(m_viewPort.Gamma());
	}

	void Viewer::SetImageLocation(const std::wstring& path) {
		std::wstring fixedPath = path;
		if(IO::DoPathExist(path))
			fixedPath += TX("\\");
		else if(!IO::DoFileExist(fixedPath))
			return;

		m_folderMonitor.Close();
		if(!m_folder.Path(IO::GetPath(fixedPath)))
			return;

		m_folderMonitor.Start(m_folder.Path());

		Img::FillCacher(m_cacher, m_folder);

		size_t imageIndex = 0;
		if (m_cacher.FindImage(path, &imageIndex))
			ActiveImage(m_cacher.GotoImage(imageIndex));
		else
			ActiveImage(m_cacher.FirstImage());
	}

	void Viewer::ShowContextMenu(Win::MouseEvent e) {
		// Disable automatic hiding while the context menu is visible
		ViewPort::CursorMode m=m_viewPort.ActiveCursorMode();
		m_viewPort.ActiveCursorMode(ViewPort::CursorShow);
		m_contextMenu.Display(e.Position);

		// Return to whatever it was originally
		m_viewPort.ActiveCursorMode(m);
	}

	void Viewer::ShowSettings() {
		m_settings->DoModeless(this);
	}

	void Viewer::OpenDirectoryInExplorer() {
		Img::Image::Ptr pImage = m_viewPort.Image();
		if (pImage.get() == 0) return;

		// Construct command line string
		std::wstring cmd(TX("explorer /e,/select,\"") + m_cacher.CurrentImageFilename() + TX("\""));

		// Start the new process
		STARTUPINFO si;
		PROCESS_INFORMATION pi;
		ZeroMemory(&si, sizeof(si));
		si.cb=sizeof(si);
		ZeroMemory(&pi, sizeof(pi));

		// Microsoft forgot "const". Copy the string to avoid casts
		boost::scoped_array<wchar_t> tmpStr(new wchar_t[cmd.length() + 1]);
		tmpStr[cmd.length()]=0;
		wcscpy_s(tmpStr.get(), (cmd.length() + 1), cmd.c_str());

		// Finally, launch explorer.exe with the nifty params
		CreateProcess(0, tmpStr.get(), 0, 0, false, 0, 0, 0, &si, &pi);
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}

	void Viewer::Sort(Img::Cacher::SortMethod m) {
		ActiveImage(m_cacher.Sort(m));
		UpdateImageInformation();
	}

	void Viewer::SettingsChanged() {
		// This one needs to be written to the registry
		Reg::Key(DWMultipleInstances, Reg::Key(DWMultipleInstances));
		Reg::Save(cg_SettingsLocation);
		m_cacher.WrapAround(Reg::Key(DWBrowseWrapAround) != 0);

		m_statusBar->Visible(Reg::Key(DWShowStatusBar) != 0);

		m_viewPort.MinificationFilter(Filter::Mode(Reg::Key(DWMinFilter)));
		m_viewPort.MagnificationFilter(Filter::Mode(Reg::Key(DWMagFilter)));

		UpdateMemoryLimits();
		m_viewPort.BackgroundColor(Img::Color::FromDWord(Reg::Key(DWBackgroundColor)));
		m_viewPort.Redraw();

		AlwaysOnTop(Reg::Key(DWAlwaysOnTop) != 0);
		ImageChanged();
	}

	void Viewer::FolderEvent(IO::FileEvent e) {
		AddNotification(e);
	}
}
