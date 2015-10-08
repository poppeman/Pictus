#include "factory_renderer.h"

#include "w32_rendertarget.h"


namespace App {
	Win::Renderer::Ptr RendererFactory::CreateRenderer() {
		return std::make_shared<Win::Renderer>();
	}
}
