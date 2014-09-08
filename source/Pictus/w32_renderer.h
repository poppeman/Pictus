#ifndef W32_RENDERER_H
#define W32_RENDERER_H

#include "illa/surface.h"
#include "w32_ddsurface.h"

namespace Win {
	class Renderer {
	public:
		virtual Img::Surface::Ptr CreateSurface()=0;
		DDSurface::Ptr CreateDDSurface();

		virtual Geom::SizeInt RenderAreaSize()=0;
		virtual Geom::RectInt GetInvalidArea();

		virtual bool PanCurrentView(const Geom::SizeInt& offset);

		bool TargetWindow(HWND hwnd);

		enum RenderStatus {
			RS_CurrentViewLost,
			RS_OK,
		};

		RenderStatus BeginRender();

		void RenderToDDSurface(DDSurface::Ptr dest, Img::Surface::Ptr source, const Geom::PointInt& zoomedImagePosition, const Geom::RectInt& destinationArea, const Img::Properties& props);
		void PresentFromDDSurface(const Geom::RectInt& destRect, DDSurface::Ptr source, const Geom::PointInt& sourceTopLeft);

		void EndRender();

		Renderer();
		virtual ~Renderer();

		typedef std::shared_ptr<Renderer> Ptr;

	protected:
		HWND TargetWindow();

	private:
		HWND m_hwnd;

		virtual DDSurface::Ptr OnCreateDDSurface()=0;

		virtual void OnRenderToDDSurface(DDSurface::Ptr dest, Img::Surface::Ptr source, const Geom::PointInt& zoomedImagePosition, const Geom::RectInt& destinationArea, const Img::Properties& props)=0;
		virtual void OnPresentFromDDSurface(const Geom::RectInt& destRect, DDSurface::Ptr source, const Geom::PointInt& sourceTopLeft)=0;

		virtual bool OnTargetWindowChanged();
		virtual RenderStatus OnBeginRender();
		virtual void OnEndRender();
	};
}

#endif
