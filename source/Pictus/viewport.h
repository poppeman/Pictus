#ifndef VIEWPORT_H
#define VIEWPORT_H

#include "illa/image.h"

#include "viewarea.h"
#include "window.h"
#include "zoomstrategy.h"
#include "timer.h"

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

		void ZoomIn();
		void ZoomOut();
		void ZoomMode(App::ZoomMode mode);
		App::ZoomMode ZoomMode() const;
		float ZoomLevel() const;

		void Rotate(Filter::RotationAngle r);

		void Brightness(int newLevel);
		int Brightness() const;

		void Contrast(int newLevel);
		int Contrast() const;

		void Gamma(int newLevel);
		int Gamma() const;

		void ActiveCursorMode(const CursorMode m);
		CursorMode ActiveCursorMode() const;

		void MagnificationFilter(Filter::FilterMode mode);
		void MinificationFilter(Filter::FilterMode mode);

		void ImageUpdated();

		// Panning
		void Pan(const Geom::SizeInt& deltaPan);

		Geom::SizeInt UnzoomedImageSize();
		Geom::SizeInt ZoomedImageSize();

		ViewPort();

	private:
		bool HandleMouseMove(Win::MouseEvent e);

		bool PerformOnCreate();

		bool PerformOnApp(int index, WPARAM wParam, LPARAM lParam);
		bool PerformOnPaint();
		bool PerformOnSize(const Geom::SizeInt& sz);
		bool PerformOnDropFiles(const StringVector& files);

		void CursorHideCallback();

		void updateCursor();

		void ZoomUpdated();

		Win::Timer m_hideTimer;

		HWND m_hParent;

		ViewArea m_viewArea;

		HWND m_hOldParent;
		CursorMode m_cursorMode;
		RECT m_oldSize;
		bool m_isCursorVisible;
		Geom::PointInt m_prevMousePos;

		ZoomStrategy m_zoom;
		float m_displayZoom;
	};
}

#endif
