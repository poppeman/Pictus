#include "StdAfx.h"
#include "viewarea.h"

#include "illa/config.h"
#include "illa/types.h"

#include "illa/surfacemgr.h"
#include "orz/sysinfo.h"

#include "getevent.h"
#include "registry.h"
#include "res_viewer.h"

#include "viewport.h"

/*namespace App {
	const wchar_t* App::ViewArea::ClassName = TX("Pictus ViewSurface");

	using namespace Reg::Keys;
	using namespace Geom;

	ViewArea::ViewArea()
		:m_isPanning(false),
		 m_imageZoom(1.0f),
		 m_currentPanMonitor(0),
		 m_magFilter(Filter::Mode::Undefined),
		 m_minFilter(Filter::Mode::Undefined)
	{
		Img::SurfaceFactory(&m_renderTarget);
		OnMouseButtonDown.connect(bind(&ViewArea::HandleMouseDown, this, _1));
		OnMouseButtonUp.connect(bind(&ViewArea::HandleMouseUp, this, _1));
		OnMouseMove.connect(bind(&ViewArea::handlePanning, this, _1));
		m_animationTimer.OnTick.connect([&]() { ImageRefreshCallback(); });
	}

	void ViewArea::Image(Img::Image::Ptr pImage) {
		m_animationTimer.Destroy();
		bool diff = (m_image != pImage);
		m_image = pImage;

		setSurface();

		if (!m_image) {
			return;
		}

		m_pan.ResizeConstraints(pImage->GetSize());
		m_pan.ResizeViewport(GetSize());

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

		InvalidateRect(Handle(), 0, false);
	}

	void ViewArea::ZoomSet(const ZoomStrategy::Result& r) {
		m_props.Zoom = r.ZoomSurface;
		m_imageZoom = r.ZoomImage;

		m_pan.ResizeConstraints(GetZoomedSize());
		m_pan.ResizeViewport(r.ZoomedSize);

		UpdateCursor();

		if (Handle() != 0) {
			InvalidateRect(Handle(), 0, false);
		}
	}

	void ViewArea::Brightness(int newLevel) {
		m_props.Brightness = newLevel;
	}

	int ViewArea::Brightness() const {
		return m_props.Brightness;
	}

	void ViewArea::Contrast(int newLevel) {
		m_props.Contrast = newLevel;
	}

	int ViewArea::Contrast() const {
		return m_props.Contrast;
	}

	void ViewArea::Gamma(int newLevel) {
		m_props.Gamma = newLevel;
	}

	int ViewArea::Gamma() const {
		return m_props.Gamma;
	}

	void ViewArea::Pan(const SizeInt& deltaPan) {
		m_pan.Pan(deltaPan);

		Update();
	}

	bool ViewArea::PerformOnCreate() {
		m_hParent = Parent()->Handle();

		WNDCLASSEX wc;
		ZeroMemory(&wc, sizeof(wc));
		wc.cbSize			= sizeof(wc);
		wc.hInstance		= GetHINSTANCE();
		wc.lpfnWndProc		= WndProcDelegate;
		wc.lpszClassName	= ClassName;
		wc.hCursor			= LoadCursor(0, IDC_ARROW);
		wc.style			= CS_DBLCLKS;
		RegisterClassEx(&wc);

		if(!ConstructWindow(RectInt(PointInt(0, 0), SizeInt(640, 480)), WS_EX_ACCEPTFILES, ClassName, TX(""), WS_CHILD | WS_VISIBLE)) return false;

		return true;
	}

	void ViewArea::BackgroundColor(const Img::Color& col) {
		m_props.BackgroundColor = col;
	}

	bool ViewArea::PerformOnApp(int index, WPARAM wp, LPARAM lp) {
		BaseWindow* pParent=Parent();
		if (pParent) return pParent->OnApp(index, wp, lp);
		return false;
	}

	void ViewArea::ImageRefreshCallback() {
		if (m_image->IsHeaderInformationValid() == false) return;

		if (m_image->Delay() != -1) {
			m_image->NextFrame();
			m_animationTimer.Create(m_image->Delay());
			Redraw();
		}
		else {
			if(m_image && m_image->IsHeaderInformationValid()) {
				ViewPort* p = static_cast<ViewPort*>(Parent());
				p->ImageUpdated();
			}
				
			bool status = m_image->IsFinished();
			Redraw();
			if(status)
				m_animationTimer.Destroy();
		}
	}

	Filter::Mode ViewArea::ActiveFilterMode() const {
		COND_STRICT(m_minFilter != Filter::Mode::Undefined, Err::InvalidCall, TX("Minification filter not set."));
		COND_STRICT(m_magFilter != Filter::Mode::Undefined, Err::InvalidCall, TX("Magnification filter not set."));

		if (m_props.Zoom < 1.0) {
			return m_minFilter;
		}

		return m_magFilter;
	}

	bool ViewArea::PerformOnPaint() {
		setSurface();

		SizeInt zoomedSize = GetZoomedSize();

		// TODO: This may call BEFORE the surface is allocated, and will thus fail when attempting to lock.
		if(m_image && m_image->IsHeaderInformationValid() && AreaNonZero(zoomedSize) && (AreaNonZero(GetSize()))) {
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

	bool ViewArea::PerformOnSize(const SizeInt&) {
		if (!m_image) {
			return false;
		}
		UpdateCursor();
		return true;
	}

	bool ViewArea::HandleMouseUp(Win::MouseEvent) {
		// Done panning, release mouse
		ReleaseCapture();

		m_isPanning = false;

		return false;
	}

	void ViewArea::UpdateCursor() {
		if (m_image == 0) {
			return;
		}

		if ((Width() >= GetZoomedSize().Width) && (Height() >= GetZoomedSize().Height)) {
			SetCursor(Win::Cursor());
		}
		else {
			SetCursor(Win::Cursor(IDC_CURSOR_PAN2));
		}
	}

	Geom::SizeInt ViewArea::GetZoomedSize() const {
		return RoundCast(GetImageUnzoomedSize() * m_imageZoom);
	}

	Geom::SizeInt ViewArea::GetSurfaceUnzoomedSize() const {
		const Img::Surface::Ptr s = m_renderTarget.CurrentSurface();
		if (s == 0) {
			return Geom::SizeInt(0, 0);
		}

		const Geom::SizeInt sz = s->GetSize();

		if (m_props.Angle == Filter::Rotate90 || m_props.Angle == Filter::Rotate270) {
			return sz.Flipped();
		}

		return sz;
	}


	Geom::SizeInt ViewArea::GetImageUnzoomedSize() const {
		if (m_image == 0) {
			return SizeInt(0, 0);
		}

		const Geom::SizeInt sz = m_image->GetSize();

		if (m_props.Angle == Filter::Rotate90 || m_props.Angle == Filter::Rotate270) {
			return sz.Flipped();
		}
		return sz;
	}

	bool ViewArea::SetRenderer(Win::Renderer::Ptr renderer) {
		m_renderTarget.SetRenderer(renderer);
		return m_renderTarget.TargetWindow(Handle());
	}

	void ViewArea::SetRedrawStrategy(Win::RedrawStrategy::Ptr strategy) {
		m_renderTarget.SetRedrawStrategy(strategy);
	}

	void ViewArea::MagnificationFilter( Filter::Mode mode ) {
		m_magFilter = mode;
	}

	void ViewArea::MinificationFilter(Filter::Mode mode) {
		m_minFilter = mode;
	}

	void ViewArea::setSurface() {
		m_renderTarget.SetSurface(m_image==0?Img::Surface::Ptr():m_image->CurrentSurface());
	}

	void ViewArea::Rotate(Filter::RotationAngle r) {
		m_props.Angle = r;
	}

	bool ViewArea::PerformOnDropFiles(const StringVector& files) {
		Win::Window* parentWindow = dynamic_cast<Win::Window*>(Parent());

		COND_STRICT(parentWindow, Err::CriticalError, TX("Parent window was not a proper window."));
		return parentWindow->OnDropFiles(files);
	}

	bool ViewArea::handlePanning(const Win::MouseEvent& e) {
		if (!((MouseStandardEvent(e) == MousePan) && m_isPanning == true))
			return false;

		COND_STRICT(m_currentPanMonitor, Err::CriticalError, TX("Current panning monitor not set."));

		PointInt globalPosition = MouseCursorPos();
		PointInt mousePosition = globalPosition;

		const Geom::RectInt& screen = m_currentPanMonitor->Region();

		if (mousePosition.X >= (screen.Right() - 1))
			mousePosition.X = screen.Left() + 1;

		if (mousePosition.X <= screen.Left())
			mousePosition.X = screen.Right() - 2;

		if (mousePosition.Y >= (screen.Bottom() - 1))
			mousePosition.Y = screen.Top() + 1;

		if (mousePosition.Y <= screen.Top())
			mousePosition.Y = screen.Bottom() - 2;

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

	bool ViewArea::HandleMouseDown(Win::MouseEvent e) {
		if (MouseStandardEvent(e) == MousePan) {
			m_isPanning	= true;
			m_oldMousePosition	= MouseCursorPos();
			m_currentPanMonitor = Win::FindMonitorAt(m_oldMousePosition);
			SetCapture(Handle());
			return true;
		}
		return false;
	}

	const Img::Color& ViewArea::BackgroundColor() const {
		return m_props.BackgroundColor;
	}
}
*/