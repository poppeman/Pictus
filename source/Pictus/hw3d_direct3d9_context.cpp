#include "hw3d_direct3d9_context.h"
#include "hw3d_direct3d9_staging_texture.h"
#include "hw3d_direct3d9_texture.h"
#include "hw3d_direct3d9_common.h"
#include "wintypes.h"

#include <orz/exception.h>

#define CONVERT_VERTEX2D_POS(v) v.Position.X, v.Position.Y, 0
#define CONVERT_VERTEX2D_TEX1(v) v.TexCoord.X, v.TexCoord.Y
#define CONVERT_VERTEX2D_VBVERTEX(v) CONVERT_VERTEX2D_POS(v), CONVERT_VERTEX2D_TEX1(v)

#define D3D_VERTEX_FVF D3DFVF_XYZ | D3DFVF_TEX1

struct vbVertex
{
	float x, y, z;
	float u, v;
};

namespace Hw3D
{
	void Direct3D9Context::Activate(wxWindow *target)
	{
		if (target == nullptr)
		{
			DO_THROW(Err::CriticalError, "OpenGlContext::Activate: target was null");
		}
		m_currentTarget = target;
		if (m_currentTarget == nullptr)
		{
			DO_THROW(Err::CriticalError, "OpenGlContext::Activate: target was not a wxGLCanvas object");
		}

		//m_context->SetCurrent(*m_currentTarget);

		D3DMATRIX mat;
		ZeroMemory(&mat, sizeof(mat));
		mat._11 = 1;
		mat._22 = 1;
		mat._33 = 1;
		mat._44 = 1;
		m_device->SetTransform(D3DTS_WORLD, &mat);
		m_device->SetTransform(D3DTS_VIEW, &mat);
	}

	void Direct3D9Context::Clear(int a, int r, int g, int b)
	{
		m_device->Clear(0, 0, D3DCLEAR_TARGET, D3DCOLOR_ARGB(a, r, g, b), 0, 0);
	}

	void Direct3D9Context::BeginDraw()
	{
		auto rect = Win::wxToRect(m_currentTarget->GetClientRect());

		/*D3DVIEWPORT9 vp;
		vp.X = rect.Left();
		vp.Y = rect.Right();
		vp.Width = rect.Width();
		vp.Height = rect.Height();
		vp.MinZ = 0.0f;
		vp.MaxZ = 1.0f;
		if (m_device->SetViewport(&vp) != D3D_OK)
		{
			DO_THROW(Err::CriticalError, "SetViewport call failed.");
		}*/

		if (m_device->BeginScene() != D3D_OK)
		{
			DO_THROW(Err::CriticalError, "BeginScene call failed.");
		}

		if (D3D_OK != m_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE))
		{
			DO_THROW(Err::CriticalError, "SetRenderState(D3DRS_CULLMODE) failed");
		}

		if (D3D_OK != m_device->SetRenderState(D3DRS_LIGHTING, false))
		{
			DO_THROW(Err::CriticalError, "SetRenderState(D3DRS_LIGHTING) failed");
		}

		m_isDrawing = true;
	}

	void Direct3D9Context::RenderQuad(const Vertex2D &a, const Vertex2D &b, const Vertex2D &c, const Vertex2D &d)
	{
		if (m_isDrawing == false)
		{
			DO_THROW(Err::CriticalError, "Device was not drawing.");
		}

		const vbVertex c_vertices[] = {
			{ CONVERT_VERTEX2D_VBVERTEX(a) },
			{ CONVERT_VERTEX2D_VBVERTEX(b) },
			{ CONVERT_VERTEX2D_VBVERTEX(c) },
			{ CONVERT_VERTEX2D_VBVERTEX(d) }
		};
		m_device->SetFVF(D3D_VERTEX_FVF);
		m_device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, c_vertices, sizeof(vbVertex));
	}

	void Direct3D9Context::EndDraw()
	{
		if (m_device == nullptr)
		{
			DO_THROW(Err::CriticalError, "Device not initialized.");
		}
		if (m_isDrawing == false)
		{
			DO_THROW(Err::CriticalError, "Device was not drawing.");
		}

		if (m_device->EndScene() != D3D_OK)
		{
			DO_THROW(Err::CriticalError, "EndScene failed.");
		}
		
		
		if (m_device->PresentEx(0, 0, 0, 0, 0) != D3D_OK)
		{
			DO_THROW(Err::CriticalError, "PresentEx failed.");
		}

		m_isDrawing = false;
	}

	void Direct3D9Context::SetTexture(int stage, std::shared_ptr<Texture> texture)
	{
		if (m_device == nullptr)
		{
			DO_THROW(Err::CriticalError, "Device not initialized.");
		}
		if (texture == nullptr)
		{
			DO_THROW(Err::InvalidParam, "texture was null.");
		}
		auto d3dTexture = std::dynamic_pointer_cast<Direct3D9Texture>(texture);
		if (d3dTexture == nullptr)
		{
			DO_THROW(Err::InvalidParam, "texture was of wrong type.");
		}

		m_device->SetTexture(stage, d3dTexture->D3DObject());
	}

	void Direct3D9Context::SetMatrix(TransformState state, const Matrix &m)
	{
		if (state == TransformState::Projection)
		{
			auto mat = MatrixToD3DMATRIX(m);
			if (m_device->SetTransform(D3DTS_PROJECTION, &mat) != D3D_OK)
			{
				DO_THROW(Err::CriticalError, "Failed setting matrix");
			}
		}
	}

	void Direct3D9Context::SendTextureRect(std::shared_ptr<StagingTexture> sourceTexture, const Geom::RectInt &sourceRect, std::shared_ptr<Texture> destinationTexture, const Geom::PointInt &destinationTopLeft)
	{
		auto d3dStaging = std::dynamic_pointer_cast<Direct3D9StagingTexture>(sourceTexture);
		LPDIRECT3DSURFACE9 sourceSurface = 0;
		if (d3dStaging->D3DObject()->GetSurfaceLevel(0, &sourceSurface) != D3D_OK)
		{
			DO_THROW(Err::CriticalError, "Failed setting surface level of staging texture");
		}

		auto d3dDestination = std::dynamic_pointer_cast<Direct3D9Texture>(destinationTexture);
		LPDIRECT3DSURFACE9 destinationSurface = 0;
		if (d3dDestination->D3DObject()->GetSurfaceLevel(0, &destinationSurface) != D3D_OK)
		{
			DO_THROW(Err::CriticalError, "Failed setting surface level of destination texture");
		}

		RECT winapi_sourceRect = RectToRECT(sourceRect);
		POINT winapi_destPoint = PointToPOINT(destinationTopLeft);

		if (m_device->UpdateSurface(sourceSurface, &winapi_sourceRect, destinationSurface, &winapi_destPoint) != D3D_OK)
		{
			DO_THROW(Err::CriticalError, "Failed updating surface");
		}

		sourceSurface->Release();
		destinationSurface->Release();
	}

	Direct3D9Context::Direct3D9Context(wxWindow *win, std::shared_ptr<IDirect3DDevice9Ex> dev) :
		m_currentTarget{ win },
		m_isInitialized{ false },
		m_isDrawing{ false },
		m_device{ dev }
	{
	}
}
