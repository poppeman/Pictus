#include "StdAfx.h"
#include "factory_renderer.h"

#include "rendertargetd3d.h"


namespace App {
	Win::Renderer::Ptr RendererFactory::CreateRenderer() {
		return std::make_shared<Win::RendererD3D>();
	}
}
