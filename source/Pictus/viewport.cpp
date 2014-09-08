#include "StdAfx.h"
#include "viewport.h"
#include "registry.h"

const wchar_t* App::ViewPort::ClassName = TX("Pictus ViewPort");

namespace App {
	using namespace Reg::Keys;
	using Geom::RectInt;
	using Geom::PointInt;
	using Geom::SizeInt;

	ViewPort::ViewPort()
		:m_isCursorVisible(true),
		m_cursorMode(CursorShow),
		m_displayZoom(1.0f)
	{
		OnMouseMove.connect([&](Win::MouseEvent e) { return HandleMouseMove(e); });
		m_hideTimer.OnTick.connect([&]() { CursorHideCallback(); });
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
		if(!ConstructWindow(RectInt(PointInt(0, 0), SizeInt(1, 1)), WS_EX_ACCEPTFILES, ClassName, TX(""), WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN)) return false;

		m_viewArea.Create(this);
		m_viewArea.Show(false);

		return true;
	}

	void ViewPort::Image(Img::Image::Ptr image) {
		m_viewArea.Image(image);

		if (image == 0 || !image->IsHeaderInformationValid())
			m_viewArea.Show(false);
		else
			m_viewArea.Show(true);

		OnSize(GetSize());
	}

	Img::Image::Ptr ViewPort::Image() const {
		return m_viewArea.Image();
	}

	void ViewPort::BackgroundColor(const Img::Color& col) {
		m_viewArea.BackgroundColor(col);
	}

	const Img::Color& ViewPort::BackgroundColor() const {
		return m_viewArea.BackgroundColor();
	}

	void ViewPort::Rotate(Filter::RotationAngle r) {
		m_viewArea.Rotate(r);
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
		m_viewArea.Pan(deltaPan);
	}

	bool ViewPort::PerformOnPaint() {
		PAINTSTRUCT ps;
		HDC dc = BeginPaint(Handle(), &ps);
		HBRUSH brush = CreateSolidBrush(Img::ABGR_ARGB(ToARGBDWORD(m_viewArea.BackgroundColor())));
		FillRect(dc, &ps.rcPaint, brush);
		DeleteObject(brush);

		EndPaint(Handle(), &ps);
		return true;
	}

	bool ViewPort::PerformOnApp(int index, WPARAM wp, LPARAM lp) {
		if(Parent()) return Parent()->OnApp(index, wp, lp);
		return false;
	}

	bool ViewPort::HandleMouseMove(Win::MouseEvent e) {
		if (ActiveCursorMode() == CursorHideAutomatic) {
			// Un-hide cursor and re-start countdown
			updateCursor();

			m_prevMousePos = e.Position;
		}
		return false;
	}

	void ViewPort::CursorHideCallback() {
		if(GetForegroundWindow() == Parent()->Handle())
			ShowCursor(false);
	}

	SizeInt ViewPort::ZoomedImageSize() {
		return m_viewArea.GetZoomedSize();
	}

	Geom::SizeInt ViewPort::UnzoomedImageSize() {
		return m_viewArea.GetSurfaceUnzoomedSize();
	}

	bool ViewPort::PerformOnSize(const SizeInt& sz) {
		// Make sure that the image is in a useful state
		m_zoom.ResizeBehaviour(App::ResizeBehaviour(Reg::Key(DWResizeBehaviour)));
		ZoomStrategy::Result r = m_zoom.CalculateViewAreaSize(GetSize(), m_viewArea.GetSurfaceUnzoomedSize(), m_viewArea.GetImageUnzoomedSize());
		m_displayZoom = r.ZoomImage;

		Geom::RectInt newRect(Geom::PointInt(0, 0), r.ZoomedSize);

		if (r.ZoomedSize.Width < sz.Width)	{
			newRect.Left((sz.Width - r.ZoomedSize.Width) / 2);
		}

		if (r.ZoomedSize.Height < sz.Height) {
			newRect.Top((sz.Height - r.ZoomedSize.Height) / 2);
		}

		//m_viewArea.ZoomSet(r.ZoomedSize, r.ZoomSurface);
		m_viewArea.ZoomSet(r);
		m_viewArea.MoveResize(newRect);

		return true;
	}

	void ViewPort::updateCursor() {
		//DestroyTimer(HideTimer);
		m_hideTimer.Destroy();

		switch(m_cursorMode) {
			case CursorHide:
				ShowCursor(false);
				break;

			case CursorShow:
				ShowCursor(true);
				break;

			case CursorHideAutomatic:
				if (MouseCursorPos() != m_prevMousePos) {
					ShowCursor(true);
				}

				m_hideTimer.Create(HideDelay);
				break;
		}
	}

	void ViewPort::Brightness( int newLevel ) {
		m_viewArea.Brightness(newLevel);
	}

	int ViewPort::Brightness() const {
		return m_viewArea.Brightness();
	}

	void ViewPort::Contrast( int newLevel ) {
		m_viewArea.Contrast(newLevel);
	}

	int ViewPort::Contrast() const {
		return m_viewArea.Contrast();
	}

	void ViewPort::Gamma( int newLevel ) {
		m_viewArea.Gamma(newLevel);
	}

	int ViewPort::Gamma() const {
		return m_viewArea.Gamma();
	}

	bool ViewPort::PerformOnDropFiles( const StringVector& files ) {
		Win::Window* parentWindow = dynamic_cast<Win::Window*>(Parent());

		COND_STRICT(parentWindow, Err::CriticalError, TX("Parent window was not a proper window."));
		return parentWindow->OnDropFiles(files);
	}

	bool ViewPort::SetRenderer( Win::Renderer::Ptr renderer ) {
		return m_viewArea.SetRenderer(renderer);
	}

	void ViewPort::SetRedrawStrategy(Win::RedrawStrategy::Ptr strategy) {
		m_viewArea.SetRedrawStrategy(strategy);
	}

	void ViewPort::MagnificationFilter( Filter::FilterMode mode ) {
		m_viewArea.MagnificationFilter(mode);
	}

	void ViewPort::MinificationFilter( Filter::FilterMode mode ) {
		m_viewArea.MinificationFilter(mode);
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
