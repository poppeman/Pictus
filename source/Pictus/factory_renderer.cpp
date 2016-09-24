#include <GL/glew.h>
#include <wx/glcanvas.h>
#include "factory_renderer.h"

#include "w32_rendertarget.h"
#include "hw3d_opengl_device.h"

namespace App {
	Win::Renderer::Ptr RendererFactory::CreateRenderer() {
		return std::make_shared<Win::Renderer>();
	}

	std::shared_ptr<Hw3D::Device> RendererFactory::CreateDevice()
	{
		return std::make_shared<Hw3D::OpenGlDevice>();
	}

	wxWindow *RendererFactory::CreateCanvas(wxWindow* parent)
	{
		int args[] = {WX_GL_RGBA, WX_GL_DOUBLEBUFFER, 0};
		return new wxGLCanvas(parent, wxID_ANY, args, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE);
	}
}
