#include "ogl_device.h"

namespace Hw3D
{
	bool Device::Initialize(wxWindow *target)
	{
		return false;
	}

	void Device::Clear(int a, int r, int g, int b)
	{

	}

	std::shared_ptr<Texture> Device::CreateTexture(const Geom::SizeInt &dimensions, Format fmt, Pool pool)
	{
		return std::shared_ptr<Texture>();
	}

	std::shared_ptr<Texture> Device::CreateRenderTarget(const Geom::SizeInt &dimensions, Format fmt)
	{
		return std::shared_ptr<Texture>();
	}

	std::shared_ptr<VertexBuffer> Device::CreateVertexBuffer(size_t sizeInBytes, int fmt)
	{
		return std::shared_ptr<VertexBuffer>();
	}

	bool Device::IsLost()
	{
		return false;
	}

	bool Device::Reset()
	{
		return false;
	}

	void Device::BeginDraw()
	{

	}

	void Device::RenderQuad(const Vertex2D &a, const Vertex2D &b, const Vertex2D &c, const Vertex2D &d)
	{

	}

	void Device::RenderQuad(int index)
	{

	}

	void Device::EndDraw()
	{

	}

	void Device::SetTexture(int stage, std::shared_ptr<Texture> texture)
	{

	}

	void Device::SetRenderTarget(std::shared_ptr<Texture> renderTarget)
	{

	}

	void Device::SetVertexBuffer(std::shared_ptr<VertexBuffer> vb, int stride)
	{

	}

	void Device::SetMatrix(TransformState state, const Matrix &m)
	{

	}

	Geom::SizeInt Device::BackBufferSize()
	{
		return Geom::Size<int>();
	}

	void Device::ResizeBackBuffer(Geom::SizeInt newSize)
	{

	}

	void Device::SendTextureRect(std::shared_ptr<Texture> sourceTexture, const Geom::RectInt &sourceRect,
								 std::shared_ptr<Texture> destinationTexture, const Geom::PointInt &destinationTopLeft)
	{

	}

}


