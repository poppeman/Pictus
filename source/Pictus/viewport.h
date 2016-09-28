#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <orz/geom.h>
#include "illa/image.h"

#include "zoomstrategy.h"
#include "timer.h"
#include "view_pan.h"

#include "w32_renderer.h"
#include "w32_redrawstrategy.h"
#include "w32_rendertarget.h"

#include "appreg.h"
#include "wintypes.h"

namespace App {
	class ViewPort {
	public:
		enum {
			HideDelay = 1000,
			//HideTimer = 0,
		};

		static const wchar_t* ClassName;

		enum CursorMode {
			CursorHideAutomatic,
			CursorShow,
			CursorHide
		};

	public:
		void SetCanvas(wxWindow* canvas);
		bool SetRenderer(Win::Renderer::Ptr renderer);
		void SetRedrawStrategy(Win::RedrawStrategy::Ptr strategy);

		void Image(Img::Image::Ptr pImage);

		Img::Image::Ptr Image() const;

		Img::Properties Properties() const;

		void BackgroundColor(const Img::Color& col);
		const Img::Color& BackgroundColor() const;

		void MouseConfig(const Reg::MouseSettings mouseConfig);

		Geom::SizeInt GetVisibleImageSize() const;

		void ZoomIn();
		void ZoomOut();
		void ZoomMode(App::ZoomMode mode);
		App::ZoomMode ZoomMode() const;
		float ZoomLevel() const;

		void ResizeBehaviour(App::ResizeBehaviour newResizeBehaviour);

		void Rotate(Filter::RotationAngle r);
		Filter::RotationAngle Rotation() const;

		void Brightness(int newLevel);
		int Brightness() const;

		void Contrast(int newLevel);
		int Contrast() const;

		void Gamma(int newLevel);
		int Gamma() const;

		void ActiveCursorMode(const CursorMode m);
		CursorMode ActiveCursorMode() const;

		void MagnificationFilter(Filter::Mode mode);
		void MinificationFilter(Filter::Mode mode);

		void ImageUpdated();

		void ResetPan(bool doReset);

		// Panning
		void Pan(const Geom::SizeInt& deltaPan);

		Geom::SizeInt OptimalViewportSize();
		Geom::SizeInt ZoomedImageSize();

		void Init();

		void Refresh();

		Geom::SizeInt GetSize();

		ViewPort(wxWindow* parent);
		ViewPort()=delete;

	private:
		::wxWindow* m_canvas = nullptr;

		Geom::PointInt MouseCursorPos();

		void HandleMouseMove(wxMouseEvent& e);

		void HandleMouseDown(wxMouseEvent& e);
		void HandleMouseDoubleClick(wxMouseEvent& e);
		void HandleMouseUp(wxMouseEvent& e);
		void ImageRefreshCallback();
		bool PerformOnCreate();

		bool PerformOnPaint();
		bool PerformOnSize(const Geom::SizeInt& sz);
		//bool PerformOnDropFiles(const StringVector& files);

		void ZoomSet(const ZoomStrategy::Result& r);

		void CursorHideCallback();

		void updateCursor();

		void ZoomUpdated();
		Filter::Mode ActiveFilterMode() const;

		void setSurface();
		Win::Timer m_hideTimer;
		Win::Timer m_animationTimer;
		Win::WinRenderTarget m_renderTarget;
		Reg::MouseSettings m_mouseConfig;

		Geom::PointInt m_oldMousePosition;
		ViewPan m_pan;

		//HWND m_hParent;

		//HWND m_hOldParent;
		bool m_isCursorVisible;
		CursorMode m_cursorMode = CursorShow;
		//RECT m_oldSize;
		Img::Image::Ptr m_image;

		Img::Properties m_props;


		ZoomStrategy m_zoom;
		float m_displayZoom = 1.0f;
		float m_imageZoom = 1.0f;
		int m_currentPanMonitor = 0;

		bool m_isPanning = false;
		bool m_resetPan = false;
		Filter::Mode m_magFilter = Filter::Mode::Undefined, m_minFilter = Filter::Mode::Undefined;
		wxWindow* m_parent = nullptr;
	};
}

#endif
