#include "StdAfx.h"
#include "w32_rs_tiled.h"

namespace Win {
	using namespace Geom;

	void RedrawStrategyTiled::OnRender( Renderer::Ptr renderer, Img::Surface::Ptr surfaceToRender, const Geom::PointInt& pan, const Img::Properties& props ) {
		Geom::SizeInt client = renderer->RenderAreaSize();

		if(m_tiles == 0) {
			m_tiles.reset(new TileManager(renderer));
			m_redrawNext = true;
		}

		m_tiles->SetViewportSize(client);

		RectInt dirtyRect = renderer->GetInvalidArea();
		if(IsPositive(dirtyRect.Dimensions())) {
			TouchTiles(dirtyRect);
		}
		if (surfaceToRender) {
			if ((memcmp(&m_prevProperties, &props, sizeof(props)) != 0) ||
				(m_prevSurface != surfaceToRender.get())) m_redrawNext = true;

			// Pre-calculate amount of panning and window resize
			Geom::SizeInt delta = m_currPosition - pan;
			Geom::SizeInt sizeDelta = m_currentSize - client;
			if(abs(sizeDelta.Width) > client.Width || abs(sizeDelta.Height) > client.Height)
				m_redrawNext = true;


			if (m_redrawNext || surfaceToRender->IsDirty())
				RenderArea(renderer, surfaceToRender, pan, Geom::RectInt(Geom::PointInt(0, 0), client), props);
			else {
				m_tiles->AddOffset(delta);

				// Fill missing pieces (horizontal and vertical bars)
				if (delta.Height > 0)
					RenderArea(renderer, surfaceToRender, pan, Geom::RectInt(Geom::PointInt(0, 0), Geom::SizeInt(client.Width, delta.Height)), props);

				if (sizeDelta.Height < 0)
					delta.Height += sizeDelta.Height;

				if (delta.Height < 0)
					RenderArea(renderer, surfaceToRender, Geom::PointInt(pan.X, pan.Y + client.Height + delta.Height), Geom::RectInt(Geom::PointInt(0, client.Height + delta.Height), Geom::SizeInt(client.Width, -delta.Height)), props);

				if (delta.Width > 0)
					RenderArea(renderer, surfaceToRender, pan, Geom::RectInt(Geom::PointInt(0, 0), Geom::SizeInt(delta.Width, client.Height)), props);

				if (sizeDelta.Width < 0)
					delta.Width += sizeDelta.Width;

				if (delta.Width < 0)
					RenderArea(renderer, surfaceToRender, Geom::PointInt(pan.X + client.Width + delta.Width, pan.Y), Geom::RectInt(Geom::PointInt(client.Width + delta.Width, 0), Geom::SizeInt(-delta.Width, client.Height)), props);
			}

			m_currPosition	= pan;
			m_currentSize	= client;
		}

		m_prevSurface	= surfaceToRender.get();
		m_prevProperties= props;
		m_redrawNext = false;
		m_tiles->Render();
	}

	RedrawStrategyTiled::RedrawStrategyTiled()
		:m_redrawNext(true),
		 m_prevSurface(0)
	{}

	void RedrawStrategyTiled::RenderArea( Renderer::Ptr renderer, Img::Surface::Ptr surface, const Geom::PointInt& zoomedImagePosition, const Geom::RectInt& destinationArea, const Img::Properties& props ) {
		// If the region actually gets cropped, there will be graphical glitching but that is better than an exception.
		RectInt croppedDestinationArea(destinationArea.Crop(RectInt(PointInt(0, 0), renderer->RenderAreaSize())));
		SizeInt wa;
		int y=croppedDestinationArea.Top();
		do {
			int x = croppedDestinationArea.Left();
			do {
				SizeInt remainingSize = -(PointInt(x, y) - croppedDestinationArea.BottomRight());

				TileManager::RequestedArea ar = m_tiles->RequestDDSurface(RectInt(PointInt(x, y), remainingSize));
				wa = ar.WriteableArea.Dimensions();

				renderer->RenderToDDSurface(
					ar.Surface,
					surface,
					zoomedImagePosition + (PointInt(x, y) - croppedDestinationArea.TopLeft()),
					ar.WriteableArea,
					props);

				x += wa.Width;
			} while(wa.Width > 0 && x < croppedDestinationArea.Right());
			y += wa.Height;
		} while(wa.Height > 0 && y < croppedDestinationArea.Bottom());
	}


	void RedrawStrategyTiled::TouchTiles( const Geom::RectInt& destinationArea ) {
		SizeInt wa;
		int y=destinationArea.Top();
		do {
			int x = destinationArea.Left();
			do {
				SizeInt remainingSize = -(PointInt(x, y) - destinationArea.BottomRight());

				TileManager::RequestedArea ar = m_tiles->RequestDDSurface(RectInt(PointInt(x, y), remainingSize));
				wa = ar.WriteableArea.Dimensions();

				x += wa.Width;
			} while(wa.Width > 0 && x < destinationArea.Right());
			y += wa.Height;
		} while(wa.Height > 0 && y < destinationArea.Bottom());
	}

	void RedrawStrategyTiled::OnReset() {
		m_tiles.reset();
	}
}
