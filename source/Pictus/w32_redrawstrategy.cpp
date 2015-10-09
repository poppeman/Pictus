#include "w32_redrawstrategy.h"

namespace Win {
	void RedrawStrategy::Render( Renderer::Ptr newRenderer, Img::Surface::Ptr surfaceToRender, const Geom::PointInt& pan, const Img::Properties& props ) {
		if (newRenderer == nullptr) {
			DO_THROW(Err::InvalidParam, TX("newRenderer was null."));
		}

		newRenderer->Angle = props.Angle;

		if (newRenderer->BeginRender(props.BackgroundColor) == Renderer::RenderStatus::CurrentViewLost) {
			Reset();
			newRenderer->BeginRender(props.BackgroundColor);
		}
		try {
			OnRender(newRenderer, surfaceToRender, pan, props);
		}
		catch (...) {
			newRenderer->EndRender();
			throw;
		}

		newRenderer->EndRender();
	}

	RedrawStrategy::~RedrawStrategy() {}

	void RedrawStrategy::Reset() {
		OnReset();
	}
}
