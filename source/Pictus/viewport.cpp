#include "StdAfx.h"
#include "viewport.h"
#include "registry.h"
#include "illa/config.h"
#include "illa/surfacemgr.h"
#include "getevent.h"

const wchar_t* App::ViewPort::ClassName = TX("Pictus ViewPort");

namespace App {
	using namespace Reg::Keys;
	using Geom::RectInt;
	using Geom::PointInt;
	using Geom::SizeInt;

	ViewPort::ViewPort()
		:m_isCursorVisible(true),
		m_cursorMode(CursorShow),
		m_displayZoom(1.0f),
		m_imageZoom(1.0f),
		m_currentPanMonitor(nullptr),
		m_isPanning(false)
	{
		Img::SurfaceFactory(&m_renderTarget);
		OnMouseMove.connect([&](Win::MouseEvent e) { return HandleMouseMove(e); });
		OnMouseButtonDown.connect([&](Win::MouseEvent e) { return HandleMouseDown(e); });
		OnMouseButtonUp.connect([&](Win::MouseEvent e) { return HandleMouseUp(e); });

		m_hideTimer.OnTick.connect([&]() { CursorHideCallback(); });
		m_animationTimer.OnTick.connect([&]() { ImageRefreshCallback(); });
	}

	bool ViewPort::PerformOnCreate() {
		m_hParent = Parent()->Handle();

		// Register the window class (if possible)
		WNDCLASSEX wc;
		ZeroMemory(&wc, sizeof(wc));
		wc.cbSize			= sizeof(wc);					// Set structure size
		wc.hCursor			= LoadCursor(0, IDC_ARROW);
		wc.hInstance		= GetHINSTANCE();				// Current instance's handle
		wc.lpfnWndProc		= WndProcDelegate;				// Use "delegate" as the wndproc
		wc.lpszClassName	= ClassName;					// Class name
		wc.style			= CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;	// Register double clicks and window resizes
		RegisterClassEx(&wc);

		// Create window without any nifty features
		return ConstructWindow(RectInt(PointInt(0, 0), SizeInt(1, 1)), WS_EX_ACCEPTFILES, ClassName, TX(""), WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN);
	}

	void ViewPort::setSurface() {
		m_renderTarget.SetSurface(m_image == 0 ? Img::Surface::Ptr() : m_image->CurrentSurface());
	}

	void ViewPort::Image(Img::Image::Ptr image) {
		m_animationTimer.Destroy();
		bool diff = (m_image != image);
		m_image = image;

		setSurface();

		if (m_image != nullptr) {
			m_pan.ResizeConstraints(image->GetSize());
			m_pan.ResizeViewport(m_renderTarget.TransformedSize());

			if (diff) {
				m_image->RestartAnimation();
			}

			if (diff && Reg::Key(DWResetPan)) {
				m_pan.Reset();
			}

			if (m_image->IsHeaderInformationValid() && m_image->Delay() != -1) {
				m_animationTimer.Create(m_image->Delay());	// It's animated, start timer and stuff
			}
			else if (m_image->IsFinished() == false) {
				m_animationTimer.Create(Img::RedrawDelay);	// Not animated or uncertain. Redraw periodically
			}

		}

		InvalidateRect(Handle(), 0, false);
		OnSize(GetSize());
	}

	Img::Image::Ptr ViewPort::Image() const {
		return m_image;
	}

	void ViewPort::BackgroundColor(const Img::Color& col) {
		m_props.BackgroundColor = col;
	}

	const Img::Color& ViewPort::BackgroundColor() const {
		return m_props.BackgroundColor;
	}

	void ViewPort::Rotate(Filter::RotationAngle r) {
		m_props.Angle = r;

		OnSize(GetSize());
	}

	void ViewPort::ActiveCursorMode(const CursorMode m) {
		m_cursorMode = m;
		updateCursor();
	}

	ViewPort::CursorMode ViewPort::ActiveCursorMode() const {
		return m_cursorMode;
	}

	void ViewPort::Pan(const SizeInt& deltaPan) {
		m_pan.Pan(deltaPan);

		Update();
	}

	void ViewPort::ImageRefreshCallback() {
		if (m_image->IsHeaderInformationValid() == false) return;

		if (m_image->Delay() != -1) {
			m_image->NextFrame();
			m_animationTimer.Create(m_image->Delay());
			Redraw();
		}
		else {
			if (m_image && m_image->IsHeaderInformationValid()) {
				ImageUpdated();
			}

			bool status = m_image->IsFinished();
			Redraw();
			if (status) {
				m_animationTimer.Destroy();
			}
		}
	}

	bool ViewPort::PerformOnPaint() {
		setSurface();

		// TODO: This may call BEFORE the surface is allocated, and will thus fail when attempting to lock.
		if (m_image && m_image->IsHeaderInformationValid() && AreaNonZero(ZoomedImageSize()) && (AreaNonZero(GetSize()))) {
			bool status = m_image->IsFinished();

			m_props.ResampleFilter = ActiveFilterMode();
			try {
				m_renderTarget.Render(m_pan.TopLeft(), m_props);
			}
			catch (Err::Exception& ex) {
				Log << L"Encountered exception:\n\n" << ex.Desc() << L"\n";
			}
			catch (...) {
				Log << "Unknown exception." << "\n";
			}
			// TODO: The exception eater here is quite horrible.

			if ((m_image->Delay() == -1) && status)
				m_animationTimer.Destroy();

			return true;
		}
		else {
			PAINTSTRUCT ps;
			BeginPaint(Handle(), &ps);
			HBRUSH brush = CreateSolidBrush(Img::ABGR_ARGB(ToARGBDWORD(m_props.BackgroundColor)));

			FillRect(ps.hdc, &ps.rcPaint, brush);
			DeleteObject(brush);
			EndPaint(Handle(), &ps);
			return true;
		}
	}

	Filter::Mode ViewPort::ActiveFilterMode() const {
		if (m_minFilter == Filter::Mode::Undefined) {
			DO_THROW(Err::InvalidCall, TX("Minification filter not set."));
		}
		if (m_magFilter == Filter::Mode::Undefined) {
			DO_THROW(Err::InvalidCall, TX("Magnification filter not set."));
		}

		if (m_props.Zoom < 1.0) {
			return m_minFilter;
		}

		return m_magFilter;
	}


	bool ViewPort::PerformOnApp(int index, WPARAM wp, LPARAM lp) {
		if (Parent()) {
			return Parent()->OnApp(index, wp, lp);
		}
		return false;
	}

	bool ViewPort::HandleMouseDown(Win::MouseEvent e) {
		if (MouseStandardEvent(e) == MousePan) {
			m_isPanning = true;
			m_oldMousePosition = MouseCursorPos();
			m_currentPanMonitor = Win::FindMonitorAt(m_oldMousePosition);
			SetCapture(Handle());
			return true;
		}
		return false;
	}

	bool ViewPort::HandleMouseUp(Win::MouseEvent) {
		// Done panning, release mouse
		ReleaseCapture();

		m_isPanning = false;

		return false;
	}

	bool ViewPort::HandleMouseMove(Win::MouseEvent e) {
		updateCursor();

		if (!((MouseStandardEvent(e) == MousePan) && m_isPanning == true)) {
			return false;
		}

		if (m_currentPanMonitor == nullptr) {
			DO_THROW(Err::CriticalError, TX("Current panning monitor not set."));
		}

		auto globalPosition = MouseCursorPos();
		auto mousePosition = globalPosition;

		const Geom::RectInt& screen = m_currentPanMonitor->Region();

		if (mousePosition.X >= (screen.Right() - 1)) {
			mousePosition.X = screen.Left() + 1;
		}

		if (mousePosition.X <= screen.Left()) {
			mousePosition.X = screen.Right() - 2;
		}

		if (mousePosition.Y >= (screen.Bottom() - 1)) {
			mousePosition.Y = screen.Top() + 1;
		}

		if (mousePosition.Y <= screen.Top()) {
			mousePosition.Y = screen.Bottom() - 2;
		}

		if (globalPosition != mousePosition) {
			SetCursorPos(mousePosition.X, mousePosition.Y);
			// Fix for RDP and similar. Sometimes (often) SetCursorPos fails silently, so we need to check manually to
			// see if it had any effect.
			POINT p;
			GetCursorPos(&p);
			mousePosition.X = p.x; mousePosition.Y = p.y;

			m_oldMousePosition = mousePosition;
		}

		SizeInt dPos(mousePosition - m_oldMousePosition);
		if ((dPos.Width != 0) || (dPos.Height != 0)) {
			m_pan.Pan(-dPos);
			m_oldMousePosition = mousePosition;

			Update();
		}

		return true;
	}

	void ViewPort::CursorHideCallback() {
		if (GetForegroundWindow() == Parent()->Handle()) {
			ShowCursor(false);
		}
	}

	SizeInt ViewPort::OptimalViewportSize() {
		return Img::CalculateUnzoomedSize(m_image, m_props.Angle);
	}

	SizeInt ViewPort::ZoomedImageSize() {
		return RoundCast(Img::CalculateUnzoomedSize(m_image, m_props.Angle) * m_imageZoom);
	}

	bool ViewPort::PerformOnSize(const SizeInt& sz) {
		// Make sure that the image is in a useful state
		m_zoom.ResizeBehaviour(App::ResizeBehaviour(Reg::Key(DWResizeBehaviour)));
		auto r = m_zoom.CalculateViewAreaSize(
			GetSize(),
			Img::CalculateUnzoomedSize(m_renderTarget.CurrentSurface(), m_props.Angle),
			Img::CalculateUnzoomedSize(m_image, m_props.Angle));
		m_displayZoom = r.ZoomImage;

		Geom::RectInt newRect(Geom::PointInt(0, 0), r.ZoomedSize);

		if (r.ZoomedSize.Width < sz.Width)	{
			newRect.Left((sz.Width - r.ZoomedSize.Width) / 2);
		}

		if (r.ZoomedSize.Height < sz.Height) {
			newRect.Top((sz.Height - r.ZoomedSize.Height) / 2);
		}

		ZoomSet(r);

		return true;
	}

	void ViewPort::ZoomSet(const ZoomStrategy::Result& r) {
		m_props.Zoom = r.ZoomSurface;
		m_imageZoom = r.ZoomImage;

		m_pan.ResizeConstraints(ZoomedImageSize());
		m_pan.ResizeViewport(r.ZoomedSize);

		//UpdateCursor();

		if (Handle() != 0) {
			InvalidateRect(Handle(), 0, false);
		}
	}

	void ViewPort::updateCursor() {
		m_hideTimer.Destroy();

		switch(m_cursorMode) {
			case CursorHide:
				ShowCursor(false);
				break;

			case CursorShow:
				ShowCursor(true);
				break;

			case CursorHideAutomatic:
				if (MouseCursorPos() != m_oldMousePosition) {
					ShowCursor(true);
				}

				m_hideTimer.Create(HideDelay);
				break;
		}
	}

	void ViewPort::Brightness(int newLevel) {
		m_props.Brightness = newLevel;
	}

	int ViewPort::Brightness() const {
		return m_props.Brightness;
	}

	void ViewPort::Contrast(int newLevel) {
		m_props.Contrast = newLevel;
	}

	int ViewPort::Contrast() const {
		return m_props.Contrast;
	}

	void ViewPort::Gamma(int newLevel) {
		m_props.Gamma = newLevel;
	}

	int ViewPort::Gamma() const {
		return m_props.Gamma;
	}

	bool ViewPort::PerformOnDropFiles( const StringVector& files ) {
		auto parentWindow = dynamic_cast<Win::Window*>(Parent());
		if (parentWindow == nullptr) {
			DO_THROW(Err::CriticalError, TX("Parent window was not a proper window."));
		}

		return parentWindow->OnDropFiles(files);
	}

	bool ViewPort::SetRenderer( Win::Renderer::Ptr renderer ) {
		m_renderTarget.SetRenderer(renderer);
		return m_renderTarget.TargetWindow(Handle());
	}

	void ViewPort::SetRedrawStrategy(Win::RedrawStrategy::Ptr strategy) {
		m_renderTarget.SetRedrawStrategy(strategy);
	}

	void ViewPort::MagnificationFilter( Filter::Mode mode ) {
		m_magFilter = mode;
	}

	void ViewPort::MinificationFilter( Filter::Mode mode ) {
		m_minFilter = mode;
	}

	void ViewPort::ImageUpdated() {
		Image(Image());
	}

	void ViewPort::ZoomUpdated() {
		OnSize(GetSize());
	}

	void ViewPort::ZoomIn() {
		if (m_zoom.ZoomIn()) {
			ZoomUpdated();
		}
	}

	void ViewPort::ZoomOut() {
		if (m_zoom.ZoomOut()) {
			ZoomUpdated();
		}
	}

	void ViewPort::ZoomMode(App::ZoomMode mode) {
		m_zoom.ZoomMode(mode);
		ZoomUpdated();
	}

	App::ZoomMode ViewPort::ZoomMode() const {
		return m_zoom.ZoomMode();
	}

	float ViewPort::ZoomLevel() const {
		return m_displayZoom;
	}
}
