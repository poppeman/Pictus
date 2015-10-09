#include "builder_viewport.h"
#include "w32_rs_tiled.h"
#include "registry.h"
#include "factory_renderer.h"

namespace App {
	using namespace Win;

	void ViewportBuilder::BuildViewport(App::ViewPort& port, Win::BaseWindow* parent, Reg::Settings settings) {
		port.Create(parent);
		port.BackgroundColor(settings.Render.BackgroundColor);

		RendererFactory rf;
		if (!port.SetRenderer(rf.CreateRenderer())) {
			DO_THROW(Err::Unsupported, L"Could not find a working renderer.");
		}

		port.SetRedrawStrategy(RedrawStrategy::Ptr(new RedrawStrategyTiled));

		port.MinificationFilter(settings.Render.MinFilter);
		port.MagnificationFilter(settings.Render.MagFilter);
		port.ResetPan(settings.View.ResetPan);
		port.ResizeBehaviour(settings.View.ResizeBehaviour);
	}
}
