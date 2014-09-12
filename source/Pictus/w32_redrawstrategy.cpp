#include "StdAfx.h"
#include "w32_redrawstrategy.h"

namespace Win {
	void RedrawStrategy::Render( Renderer::Ptr newRenderer, Img::Surface::Ptr surfaceToRender, const Geom::PointInt& pan, const Img::Properties& props ) {
		COND_STRICT(newRenderer, Err::InvalidParam, TX("newRenderer was null."));

		if(newRenderer->BeginRender(props.BackgroundColor) == Renderer::RS_CurrentViewLost) {
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
