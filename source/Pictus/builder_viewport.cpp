#include "StdAfx.h"
#include "builder_viewport.h"
#include "w32_rs_tiled.h"
#include "registry.h"
#include "factory_renderer.h"

namespace App {
	using namespace Win;
	using namespace Reg::Keys;

	void ViewportBuilder::BuildViewport(App::ViewPort& port, Win::BaseWindow* parent) {
		port.Create(parent);
		port.BackgroundColor(Img::Color::FromDWord(Reg::Key(DWBackgroundColor)));

		RendererFactory rf;
		if(!port.SetRenderer(rf.CreateRenderer())) DO_THROW(Err::Unsupported, TX("Could not find a working renderer."));

		port.SetRedrawStrategy(RedrawStrategy::Ptr(new RedrawStrategyTiled));

		port.MinificationFilter(Filter::FilterMode(Reg::Key(DWMinFilter)));
		port.MagnificationFilter(Filter::FilterMode(Reg::Key(DWMagFilter)));
	}
}
