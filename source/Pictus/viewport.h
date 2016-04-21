#ifndef VIEWPORT_H
#define VIEWPORT_H

#include "illa/image.h"

#include "window.h"
#include "zoomstrategy.h"
#include "timer.h"
#include "view_pan.h"

#include "w32_renderer.h"
#include "w32_redrawstrategy.h"
#include "w32_rendertarget.h"

#include "monitor.h"
#include "appreg.h"

namespace App {
	class ViewPort:public Win::Window {
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
		bool SetRenderer(Win::Renderer::Ptr renderer);
		void SetRedrawStrategy(Win::RedrawStrategy::Ptr strategy);

		void Image(Img::Image::Ptr pImage);

		Img::Image::Ptr Image() const;

		void BackgroundColor(const Img::Color& col);
		const Img::Color& BackgroundColor() const;

		void MouseConfig(const Reg::MouseSettings mouseConfig);

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

		ViewPort();

	private:
		bool HandleMouseMove(Win::MouseEvent e);
		bool HandleMouseDown(Win::MouseEvent e);
		bool HandleMouseUp(Win::MouseEvent e);
		void ImageRefreshCallback();
		bool PerformOnCreate();

		bool PerformOnApp(int index, WPARAM wParam, LPARAM lParam);
		bool PerformOnPaint();
		bool PerformOnSize(const Geom::SizeInt& sz);
		bool PerformOnDropFiles(const StringVector& files);

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
		bool m_isPanning;
		const Win::Monitor* m_currentPanMonitor;

		HWND m_hParent;

		HWND m_hOldParent;
		CursorMode m_cursorMode;
		RECT m_oldSize;
		bool m_isCursorVisible;
		Img::Image::Ptr m_image;

		Img::Properties m_props;

		bool m_resetPan;

		ZoomStrategy m_zoom;
		float m_displayZoom;
		float m_imageZoom;
		Filter::Mode m_magFilter, m_minFilter;
	};
}

#endif
