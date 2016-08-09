#include "viewport.h"
#include "registry.h"
#include "illa/config.h"
#include "illa/surfacemgr.h"
#include "getevent.h"
#include <wx/display.h>
#include <wx/glcanvas.h>

namespace App {
	using Geom::RectInt;
	using Geom::PointInt;
	using Geom::SizeInt;

	ViewPort::ViewPort(wxWindow* parent):
		m_canvas{nullptr},
		m_isCursorVisible(true),
		m_cursorMode(CursorShow),
		m_displayZoom(1.0f),
		m_imageZoom(1.0f),
		m_currentPanMonitor(-1),
		m_isPanning(false),
		m_resetPan(false),
		m_parent(parent)
	{
		Img::SurfaceFactory(&m_renderTarget);
	}

	bool ViewPort::PerformOnCreate() {
		m_canvas->SetPosition({0, 0});
		m_canvas->SetSize(320, 200);
		m_canvas->Show(true);
		m_canvas->GetSize();
		/*m_hParent = Parent()->Handle();

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
		return ConstructWindow(RectInt(PointInt(0, 0), SizeInt(1, 1)), WS_EX_ACCEPTFILES, ClassName, L"", WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN);*/
		return true;
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
			m_props.MetaAngle = Filter::RotationAngle::RotateDefault;

			auto meta = m_image->GetMetadata();
			if (meta != nullptr) {
				if (meta->Field[Metadata::FieldIdentifier::Orientation] != nullptr) {
					switch (meta->Field[Metadata::FieldIdentifier::Orientation]->ToInteger()) {
					case 1:
						m_props.MetaAngle = Filter::RotationAngle::RotateDefault;
						break;
					case 2:
						m_props.MetaAngle = Filter::RotationAngle::FlipX;
						break;
					case 3:
						m_props.MetaAngle = Filter::RotationAngle::Rotate180;
						break;
					case 4:
						m_props.MetaAngle = Filter::RotationAngle::FlipY;
						break;
					case 5:
						m_props.MetaAngle = Filter::RotationAngle::Rotate270FlipY;
						break;
					case 6:
						m_props.MetaAngle = Filter::RotationAngle::Rotate90;
						break;
					case 7:
						m_props.MetaAngle = Filter::RotationAngle::Rotate90FlipY;
						break;
					case 8:
						m_props.MetaAngle = Filter::RotationAngle::Rotate270;
						break;
					}
				}
			}

			m_pan.ResizeConstraints(image->GetSize());
			m_pan.ResizeViewport(m_renderTarget.TransformedSize());

			if (diff) {
				m_image->RestartAnimation();
			}

			if (diff && m_resetPan) {
				m_pan.Reset();
			}

			if (m_image->IsHeaderInformationValid() && m_image->Delay() != -1) {
				m_animationTimer.Create(m_image->Delay());	// It's animated, start timer and stuff
			}
			else if (m_image->IsFinished() == false) {
				m_animationTimer.Create(Img::RedrawDelay);	// Not animated or uncertain. Redraw periodically
			}

		}

		m_canvas->Refresh();
		/*InvalidateRect(Handle(), 0, false);
		OnSize(GetSize());*/
	}

	Img::Image::Ptr ViewPort::Image() const {
		return m_image;
	}

	void ViewPort::BackgroundColor(const Img::Color& col) {
		m_props.BackgroundColor = col;
		m_canvas->SetBackgroundColour(wxColour(col.R, col.G, col.B, col.A));
	}

	const Img::Color& ViewPort::BackgroundColor() const {
		return m_props.BackgroundColor;
	}

	void ViewPort::Rotate(Filter::RotationAngle r) {
		m_props.RequestedAngle = r;

		PerformOnSize(GetSize());
	}

	Filter::RotationAngle ViewPort::Rotation() const {
		return m_props.RequestedAngle;
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

		m_canvas->Update();
	}

	void ViewPort::ImageRefreshCallback() {
		if (m_image->IsHeaderInformationValid() == false) return;

		if (m_image->Delay() != -1) {
			m_image->NextFrame();
			m_animationTimer.Create(m_image->Delay());
			m_canvas->Refresh();
		}
		else {
			if (m_image && m_image->IsHeaderInformationValid()) {
				ImageUpdated();
			}

			bool status = m_image->IsFinished();
			m_canvas->Refresh();
			if (status) {
				m_animationTimer.Destroy();
			}
		}
	}

	bool ViewPort::PerformOnPaint() {
		setSurface();

		// TODO: This may call BEFORE the surface is allocated, and will thus fail when attempting to lock.
		if (m_image && m_image->IsHeaderInformationValid() && AreaNonZero(ZoomedImageSize()) && (Geom::AreaNonZero(GetSize()))) {
			bool status = m_image->IsFinished();

			m_props.ResampleFilter = ActiveFilterMode();
			m_renderTarget.Render(m_pan.TopLeft(), m_props);

			if ((m_image->Delay() == -1) && status)
				m_animationTimer.Destroy();

			}
			else {
				m_renderTarget.Clear(m_props.BackgroundColor);
			}

			// Direct3D9 Ex (with FLIPEX) doesn't seem to validate the window the same way as vanilla D3D9.
			// Therefore we do it ourselves to prevent WM_PAINT spam.
			//ValidateRect(Handle(), nullptr);
			return true;
		}

	Filter::Mode ViewPort::ActiveFilterMode() const {
		if (m_minFilter == Filter::Mode::Undefined) {
			DO_THROW(Err::InvalidCall, "Minification filter not set.");
		}
		if (m_magFilter == Filter::Mode::Undefined) {
			DO_THROW(Err::InvalidCall, "Magnification filter not set.");
		}

		if (m_props.Zoom < 1.0) {
			return m_minFilter;
		}

		return m_magFilter;
	}

	void ViewPort::HandleMouseDown(wxMouseEvent& evt) {
		if (MouseStandardEvent(Win::MouseEvent(evt), m_mouseConfig) == MousePan) {
			m_isPanning = true;
			m_oldMousePosition = MouseCursorPos();
			m_currentPanMonitor = wxDisplay::GetFromPoint(Win::PointToWx(m_oldMousePosition));
			m_canvas->CaptureMouse();
			return;
		}
		evt.ResumePropagation(1);
		evt.Skip();
	}

	void ViewPort::HandleMouseDoubleClick(wxMouseEvent &e)
	{
		e.ResumePropagation(1);
		e.Skip();
	}

	void ViewPort::HandleMouseUp(wxMouseEvent& evt) {
		// Done panning, release mouse
		if(m_isPanning)
		{
			m_canvas->ReleaseMouse();
		}

		m_isPanning = false;

		evt.ResumePropagation(1);
		evt.Skip();
	}

	void ViewPort::HandleMouseMove(wxMouseEvent& evt) {
		updateCursor();

		if (!((MouseStandardEvent(evt, m_mouseConfig)) && m_isPanning == true)) {
			evt.ResumePropagation(1);
			evt.Skip();
			return;
		}

		if (m_currentPanMonitor == wxNOT_FOUND) {
			DO_THROW(Err::CriticalError, "Current panning monitor not set.");
		}

		auto globalPosition = MouseCursorPos();
		auto mousePosition = globalPosition;

		auto screen = Win::wxToRect(wxDisplay(m_currentPanMonitor).GetClientArea());

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
			/*SetCursorPos(mousePosition.X, mousePosition.Y);
			// Fix for RDP and similar. Sometimes (often) SetCursorPos fails silently, so we need to check manually to
			// see if it had any effect.
			POINT p;
			GetCursorPos(&p);
			mousePosition.X = p.x; mousePosition.Y = p.y;

			m_oldMousePosition = mousePosition;*/
		}

		SizeInt dPos(mousePosition - m_oldMousePosition);
		if ((dPos.Width != 0) || (dPos.Height != 0)) {
			m_pan.Pan(-dPos);
			m_oldMousePosition = mousePosition;

			m_canvas->Update();
		}
	}

	void ViewPort::CursorHideCallback() {
		// TODO: Reimplement cursor hiding
		/*if (GetForegroundWindow() == Parent()->Handle()) {
			ShowCursor(false);
		}*/
	}

	SizeInt ViewPort::OptimalViewportSize() {
		return Img::CalculateUnzoomedSize(m_image, m_props.FinalAngle());
	}

	SizeInt ViewPort::ZoomedImageSize() {
		return RoundCast(Img::CalculateUnzoomedSize(m_image, m_props.FinalAngle()) * m_imageZoom);
	}

	bool ViewPort::PerformOnSize(const SizeInt& sz) {
		// Make sure that the image is in a useful state
		auto r = m_zoom.CalculateViewAreaSize(
			GetSize(),
			Img::CalculateUnzoomedSize(m_renderTarget.CurrentSurface(), m_props.FinalAngle()),
			Img::CalculateUnzoomedSize(m_image, m_props.FinalAngle()));
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

		m_canvas->Refresh();
		/*if (Handle() != 0) {
			InvalidateRect(Handle(), 0, false);
		}*/
	}

	void ViewPort::updateCursor() {
		m_hideTimer.Destroy();

		// TODO: Reimplement cursor hiding
		/*switch(m_cursorMode) {
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
		}*/
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

	/*bool ViewPort::PerformOnDropFiles( const StringVector& files ) {
		auto parentWindow = dynamic_cast<Win::Window*>(Parent());
		if (parentWindow == nullptr) {
			DO_THROW(Err::CriticalError, "Parent window was not a proper window.");
		}

		return parentWindow->OnDropFiles(files);
	}*/

	bool ViewPort::SetRenderer( Win::Renderer::Ptr renderer ) {
		m_renderTarget.SetRenderer(renderer);
		m_renderTarget.TargetWindow(m_canvas);
		return true;
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
		PerformOnSize(GetSize());
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

	void ViewPort::ResetPan(bool doReset) {
		m_resetPan = doReset;
	}

	void ViewPort::MouseConfig(const Reg::MouseSettings mouseConfig) {
		m_mouseConfig = mouseConfig;
	}

	void ViewPort::ResizeBehaviour(App::ResizeBehaviour newResizeBehaviour) {
		m_zoom.ResizeBehaviour(newResizeBehaviour);
	}

	PointInt ViewPort::MouseCursorPos()
	{
		auto state = wxGetMouseState();
		auto pos = state.GetPosition();
		return {pos.x, pos.y};
	}

	void ViewPort::Init()
	{
		PerformOnCreate();
	}

	Geom::SizeInt ViewPort::GetSize()
	{
		return Win::wxToSize(m_canvas->GetSize());
	}

	void ViewPort::SetRect(Geom::RectInt rect)
	{
		m_canvas->SetSize(rect.Left(), rect.Top(), rect.Width(), rect.Height());
	}

	void ViewPort::Refresh()
	{
		m_canvas->Refresh();
	}

	void ViewPort::SetCanvas(wxWindow* canvas)
	{
		if(m_canvas != nullptr)
		{
			delete m_canvas;
			m_canvas = nullptr;
		}

		m_canvas = canvas;
		// TODO: Bind the other events
		m_canvas->Bind(wxEVT_LEFT_DOWN, &ViewPort::HandleMouseDown, this);
		m_canvas->Bind(wxEVT_LEFT_DCLICK, &ViewPort::HandleMouseDoubleClick, this);
		m_canvas->Bind(wxEVT_LEFT_UP, &ViewPort::HandleMouseUp, this);

		m_canvas->Bind(wxEVT_MIDDLE_DOWN, &ViewPort::HandleMouseDown, this);
		m_canvas->Bind(wxEVT_MIDDLE_DCLICK, &ViewPort::HandleMouseDoubleClick, this);
		m_canvas->Bind(wxEVT_MIDDLE_UP, &ViewPort::HandleMouseUp, this);

		m_canvas->Bind(wxEVT_RIGHT_DOWN, &ViewPort::HandleMouseDown, this);
		m_canvas->Bind(wxEVT_RIGHT_DCLICK, &ViewPort::HandleMouseDoubleClick, this);
		m_canvas->Bind(wxEVT_RIGHT_UP, &ViewPort::HandleMouseUp, this);

		m_canvas->Bind(wxEVT_AUX1_DOWN, &ViewPort::HandleMouseDown, this);
		m_canvas->Bind(wxEVT_AUX1_DCLICK, &ViewPort::HandleMouseDoubleClick, this);
		m_canvas->Bind(wxEVT_AUX1_UP, &ViewPort::HandleMouseUp, this);

		m_canvas->Bind(wxEVT_AUX2_DOWN, &ViewPort::HandleMouseDown, this);
		m_canvas->Bind(wxEVT_AUX2_DCLICK, &ViewPort::HandleMouseDoubleClick, this);
		m_canvas->Bind(wxEVT_AUX2_UP, &ViewPort::HandleMouseUp, this);

		m_canvas->Bind(wxEVT_MOTION, &ViewPort::HandleMouseMove, this);
		m_canvas->Bind(wxEVT_MOUSEWHEEL, &ViewPort::HandleMouseDown, this);

		m_canvas->Bind(wxEVT_PAINT, [&](wxPaintEvent& evt) { PerformOnPaint(); });
		m_canvas->Bind(wxEVT_SIZE, [&](wxSizeEvent& evt) { PerformOnSize(Win::wxToSize(evt.GetSize())); });
		m_canvas->Bind(wxEVT_SIZING, [&](wxSizeEvent& evt) { PerformOnSize(Win::wxToSize(evt.GetSize())); });

		m_canvas->Bind(wxEVT_KEY_DOWN, [&](wxKeyEvent& e) {
			e.ResumePropagation(1);
			e.Skip();
		});

		m_renderTarget.TargetWindow(m_canvas);

		//m_hideTimer.OnTick.connect([&]() { CursorHideCallback(); });
		//m_animationTimer.OnTick.connect([&]() { ImageRefreshCallback(); });
	}
}
