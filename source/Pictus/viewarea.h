#ifndef VIEWAREA_H
#define VIEWAREA_H

#include "illa/image.h"

#include "window.h"
#include "monitor.h"
#include "w32_rendertarget.h"
#include "view_pan.h"
#include "timer.h"
#include "zoomstrategy.h"

namespace App {
	/*class ViewArea:public Win::Window {
	public:
		static const wchar_t* ClassName;

	public:
		bool SetRenderer(Win::Renderer::Ptr renderer);
		void SetRedrawStrategy(Win::RedrawStrategy::Ptr strategy);

		void BackgroundColor(const Img::Color& col);
		const Img::Color& BackgroundColor() const;
		void Image(Img::Image::Ptr pImage);
		Img::Image::Ptr Image() const { return m_image; }

		Geom::SizeInt GetZoomedSize() const;
		Geom::SizeInt GetSurfaceUnzoomedSize() const;
		Geom::SizeInt GetImageUnzoomedSize() const;

		// Zooming
		void ZoomSet(const ZoomStrategy::Result& r);

		// Adjust
		void Brightness(int newLevel);
		int Brightness() const;
		void Contrast(int newLevel);
		int Contrast() const;
		void Gamma(int newLevel);
		int Gamma() const;

		void Rotate(Filter::RotationAngle r);

		// Panning
		void Pan(const Geom::SizeInt& deltaPan);

		void MagnificationFilter(Filter::Mode mode);
		void MinificationFilter(Filter::Mode mode);

		ViewArea();

	private:
		bool HandleMouseDown(Win::MouseEvent e);
		bool HandleMouseUp(Win::MouseEvent e);

		bool PerformOnCreate();

		bool PerformOnApp(int index, WPARAM wParam, LPARAM lParam);
		void ImageRefreshCallback();
		bool PerformOnPaint();
		bool PerformOnSize(const Geom::SizeInt& sz);
		bool PerformOnEraseBackground(HDC hdc)  { hdc; return true; }

		bool PerformOnDropFiles(const StringVector& files);

		void UpdateCursor();

		void setSurface();

		bool handlePanning(const Win::MouseEvent& e);
		Filter::Mode ActiveFilterMode() const;

		Win::WinRenderTarget m_renderTarget;

		Geom::PointInt m_oldMousePosition;
		ViewPan m_pan;
		bool m_isPanning;

		Win::Timer m_animationTimer;

		const Win::Monitor* m_currentPanMonitor;

		Img::Image::Ptr m_image;

		HWND m_hParent;

		Img::Properties m_props;
		float m_imageZoom;
		Filter::Mode m_magFilter, m_minFilter;
	};*/
}

#endif
