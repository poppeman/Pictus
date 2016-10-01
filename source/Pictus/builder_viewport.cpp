#include "builder_viewport.h"
#include "w32_rs_tiled.h"
#include "registry.h"
#include "factory_renderer.h"

namespace App {
	using namespace Win;

	void ViewportBuilder::BuildViewport(App::ViewPort& port, wxWindow* parent, const Reg::Settings& settings) {
		//port.Create(parent);
		RendererFactory rf;
		auto device = rf.CreateDevice();
		auto renderer = rf.CreateRenderer();
		renderer->Device(device);

		port.SetCanvas(rf.CreateCanvas(parent));
		port.SetRenderer(renderer);

		port.BackgroundColor(settings.Render.BackgroundColor);
		port.SetRedrawStrategy(std::make_shared<RedrawStrategyTiled>());

		port.MinificationFilter(settings.Render.MinFilter);
		port.MagnificationFilter(settings.Render.MagFilter);
		port.ResetPan(settings.View.ResetPan);
		port.ResizeBehaviour(settings.View.ResizeBehaviour);
		port.Init();
	}
}
