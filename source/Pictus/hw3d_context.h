#ifndef PICTUS_HW3D_CONTEXT_H
#define PICTUS_HW3D_CONTEXT_H

#include <memory>

#include "hw3d_texture.h"
#include "hw3d_staging_texture.h"
#include "hw3d_vertex2d.h"
#include "orz/matrix.h"
#include <wx/window.h>

namespace Hw3D
{
	class Context
	{
	public:
		virtual void Activate(wxWindow* target)=0;
		virtual void Clear(int a, int r, int g, int b)=0;

		virtual void SetTexture(int stage, std::shared_ptr<Texture> texture)=0;
		virtual void SetRenderTarget(std::shared_ptr<Texture> renderTarget)=0;

		virtual void BeginDraw()=0;
		virtual void RenderQuad(const Vertex2D& a, const Vertex2D& b, const Vertex2D& c, const Vertex2D& d)=0;
		virtual void EndDraw()=0;

		virtual void SetMatrix(TransformState state, const Matrix& m)=0;

		virtual void SendTextureRect(std::shared_ptr<StagingTexture> sourceTexture, const Geom::RectInt& sourceRect, std::shared_ptr<Texture> destinationTexture, const Geom::PointInt& destinationTopLeft)=0;
	};
}

#endif
