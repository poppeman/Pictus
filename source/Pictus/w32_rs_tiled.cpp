#include "StdAfx.h"
#include "w32_rs_tiled.h"

namespace Win {
	using namespace Geom;

	void RedrawStrategyTiled::OnRender(Renderer::Ptr renderer, Img::Surface::Ptr surfaceToRender, Geom::PointInt pan, Img::Properties props) {
		if (surfaceToRender == nullptr) {
			return;
		}

		auto client = RoundCast(surfaceToRender->GetSize() * props.Zoom);

		// TODO: Recalculate pan according to rotation
		//pan = renderer->Transform(pan, client);

		//Geom::SizeInt client = renderer->RenderAreaSize();

		if (m_tiles == nullptr) {
			m_tiles = std::make_shared<TileManager>(renderer);
			m_redrawNext = true;
		}

		m_tiles->SetViewportSize(client);

		if (IsPositive(client) == false) {
			return;
		}

		auto dirtyRect = RectInt({ 0, 0 }, client);
		if (IsPositive(dirtyRect.Dimensions())) {
			TouchTiles(dirtyRect);
		}

		// Only force a full redraw if relevant properties have changed. Angle for instance
		// does not require a redraw, as rotation is handled entirely during rendering.
		if (props.BackgroundColor != m_prevProperties.BackgroundColor ||
			props.Brightness != m_prevProperties.Brightness ||
			props.Contrast != m_prevProperties.Contrast ||
			props.Gamma != m_prevProperties.Gamma ||
			props.ResampleFilter != m_prevProperties.ResampleFilter ||
			props.RetainAlpha != m_prevProperties.RetainAlpha ||
			props.Zoom != m_prevProperties.Zoom ||
			m_prevSurface != surfaceToRender.get()) {
			m_redrawNext = true;
		}

		// Pre-calculate amount of panning and window resize
		auto delta = m_currPosition - pan;
		auto sizeDelta = m_currentSize - client;
		if (abs(sizeDelta.Width) > client.Width || abs(sizeDelta.Height) > client.Height) {
			//m_redrawNext = true;
		}

		if (m_redrawNext || surfaceToRender->IsDirty()) {
			RenderArea(renderer, surfaceToRender, pan, { { 0, 0 }, client }, props);
		}
		else {
			m_tiles->AddOffset(delta);

			// Fill missing pieces (horizontal and vertical bars)
			if (delta.Height > 0) {
				RenderArea(renderer, surfaceToRender, pan, { { 0, 0 }, Geom::SizeInt(client.Width, delta.Height) }, props);
			}

			if (sizeDelta.Height < 0) {
				delta.Height += sizeDelta.Height;
			}

			if (delta.Height < 0) {
				RenderArea(renderer, surfaceToRender, { pan.X, pan.Y + client.Height + delta.Height }, { { 0, client.Height + delta.Height }, Geom::SizeInt(client.Width, -delta.Height) }, props);
			}

			if (delta.Width > 0) {
				RenderArea(renderer, surfaceToRender, pan, { { 0, 0 }, Geom::SizeInt(delta.Width, client.Height) }, props);
			}

			if (sizeDelta.Width < 0) {
				delta.Width += sizeDelta.Width;
			}

			if (delta.Width < 0) {
				RenderArea(renderer, surfaceToRender, { pan.X + client.Width + delta.Width, pan.Y }, { { client.Width + delta.Width, 0 }, Geom::SizeInt(-delta.Width, client.Height) }, props);
			}
		}

		m_currPosition = pan;
		m_currentSize = client;

		m_prevSurface = surfaceToRender.get();
		m_prevProperties = props;
		m_redrawNext = false;

		auto offset = RoundCast((renderer->TransformedRenderAreaSize() - client) * 0.5f);
		if (offset.Width < 0) offset.Width = 0;
		if (offset.Height < 0) offset.Height = 0;

		m_tiles->Render(offset);
	}

	RedrawStrategyTiled::RedrawStrategyTiled()
		:m_redrawNext(true),
		 m_prevSurface(nullptr)
	{}

	void RedrawStrategyTiled::RenderArea(Renderer::Ptr renderer, Img::Surface::Ptr surface, Geom::PointInt zoomedImagePosition, Geom::RectInt destinationArea, Img::Properties props) {
		// We intentionally remove any rotation set here. The rotation will instead be handled by the GPU when the tiles are rendered to screen.
		props.Angle = Filter::RotationAngle::RotateDefault;

		// If the region actually gets cropped, there will be graphical glitching but that is better than an exception.
		auto croppedDestinationArea(destinationArea.Crop({ { 0, 0 }, RoundCast(surface->GetSize() * props.Zoom) }));
		SizeInt wa;
		int y = croppedDestinationArea.Top();
		do {
			int x = croppedDestinationArea.Left();
			do {
				auto remainingSize = -(PointInt(x, y) - croppedDestinationArea.BottomRight());

				auto ar = m_tiles->RequestDDSurface({ { x, y }, remainingSize });
				wa = ar.WriteableArea.Dimensions();

				renderer->RenderToDDSurface(
					ar.Surface,
					surface,
					zoomedImagePosition + (PointInt(x, y) - croppedDestinationArea.TopLeft()),
					ar.WriteableArea,
					props);

				x += wa.Width;
			} while (wa.Width > 0 && x < croppedDestinationArea.Right());
			y += wa.Height;
		} while (wa.Height > 0 && y < croppedDestinationArea.Bottom());
	}


	void RedrawStrategyTiled::TouchTiles(Geom::RectInt destinationArea) {
		SizeInt wa;
		int y = destinationArea.Top();
		do {
			int x = destinationArea.Left();
			do {
				auto remainingSize = -(PointInt(x, y) - destinationArea.BottomRight());

				auto ar = m_tiles->RequestDDSurface(RectInt(PointInt(x, y), remainingSize));
				wa = ar.WriteableArea.Dimensions();

				x += wa.Width;
			} while (wa.Width > 0 && x < destinationArea.Right());
			y += wa.Height;
		} while (wa.Height > 0 && y < destinationArea.Bottom());
	}

	void RedrawStrategyTiled::OnReset() {
		m_tiles.reset();
	}
}
