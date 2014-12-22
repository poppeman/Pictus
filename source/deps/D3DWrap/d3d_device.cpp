#include "StdAfx.h"

#include "d3d_device.h"
#include "d3d_common.h"
#include "int_d3d_types.h"
#include "int_d3d_base.h"

namespace D3D {
	using Geom::SizeInt;

	bool Device::Initialize(HWND hwndFocus, bool doWindowed, int device) {
		// Can occur of Direct3D9 is not installed.
		if (m_d3d == 0) throw Err::Direct3DError(L"Direct3D 9 could not be found");

		ZeroMemory( &m_presentParams, sizeof(m_presentParams) );

		m_presentParams.Windowed = doWindowed;
		m_presentParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
		//m_presentParams.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
		m_presentParams.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
		//m_presentParams.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
		//m_presentParams.hDeviceWindow = hwndFocus;
		m_presentParams.BackBufferCount		= 1;

		if (doWindowed) {
			m_presentParams.BackBufferFormat	= D3DFMT_UNKNOWN;
			m_presentParams.BackBufferWidth		= 0;
			m_presentParams.BackBufferHeight	= 0;
		}
		else {
			m_presentParams.BackBufferFormat	= D3DFMT_X8R8G8B8;
			m_presentParams.BackBufferWidth		= 128;
			m_presentParams.BackBufferHeight	= 128;
		}

#if 1
		auto hr = m_d3d->CreateDevice(
			device,
			D3DDEVTYPE_HAL,
			hwndFocus,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&m_presentParams,
			&m_device);

		switch (hr) {
		case D3DERR_DEVICELOST:
			throw Err::Direct3DError(L"Direct3D Device was lost.");
		case D3DERR_INVALIDCALL:
			throw Err::Direct3DError(L"Invalid call during CreateDevice.");
		case D3DERR_NOTAVAILABLE:
			throw Err::Direct3DError(L"Direct3D device does not support required.");
		case D3DERR_OUTOFVIDEOMEMORY:
			throw Err::Direct3DError(L"Not enough video memory to initialize Direct3D.");
		}

#else
		bool foundPerf = false;
		for(UINT Adapter=0;Adapter<m_d3d->GetAdapterCount();Adapter++) {
			D3DADAPTER_IDENTIFIER9 Identifier;
			m_d3d->GetAdapterIdentifier(Adapter, 0, &Identifier);
			if(strstr(Identifier.Description,"PerfHUD") != 0) {
				foundPerf = true;
				m_d3d->CreateDevice(
					Adapter,
					D3DDEVTYPE_REF,
					hwndFocus,
					D3DCREATE_HARDWARE_VERTEXPROCESSING,
					&m_presentParams,
					&m_device);
			}
			if (foundPerf) break;
		}

		if (!foundPerf)
			::MessageBox(0, L"NO PERFHUD", 0, MB_OK);
		else
			::MessageBox(0, L"Found perf", 0, MB_OK);

#endif

		if (m_device == 0)
			return false;

		return true;
	}


	void Device::Clear( int a, int r, int g, int b ) {
		m_device->Clear(0, 0, D3DCLEAR_TARGET, D3DCOLOR_ARGB(a, r, g, b), 0, 0);
	}

	SwapChain::Ptr Device::CreateSwapChain(HWND hwnd) {
		if (m_device == 0) {
			DO_THROW(Err::CriticalError, TX("Device not initialized."));
		}

		// Recreate swap chain if needed
		LPDIRECT3DSWAPCHAIN9 swapChain = 0;
		m_device->GetSwapChain(0, &swapChain);

		D3DPRESENT_PARAMETERS pp;
		ZeroMemory(&pp, sizeof(pp));

		if (swapChain) {
			swapChain->GetPresentParameters(&pp);
			swapChain->Release();
		}

		pp.Windowed = TRUE;
		pp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		pp.BackBufferFormat = D3DFMT_UNKNOWN;
		pp.BackBufferCount = 1;
		pp.BackBufferWidth = 0;
		pp.BackBufferHeight = 0;
		pp.hDeviceWindow = hwnd;

		if (m_device->CreateAdditionalSwapChain(&pp, &swapChain) != D3D_OK) {
			DO_THROW(Err::Direct3DError, TX("Couldn't create new swap chain."));
		}

		return std::make_shared<SwapChain>(swapChain, hwnd);
	}

	Texture::Ptr Device::CreateTexture(const Geom::SizeInt& dimensions, D3DFORMAT fmt, D3DPOOL pool) {
		if (m_device == 0) {
			DO_THROW(Err::CriticalError, TX("Device not initialized."));
		}
		LPDIRECT3DTEXTURE9 texture;

		if (m_device->CreateTexture(dimensions.Width, dimensions.Height, 1, 0, fmt, pool, &texture, 0) != D3D_OK) {
			DO_THROW(Err::Direct3DError, TX("Couldn't allocate texture."));
		}

		return Texture::Ptr(new Texture(texture));
	}

	Texture::Ptr Device::CreateRenderTarget(const Geom::SizeInt& dimensions, D3DFORMAT fmt) {
		if (m_device == 0) {
			DO_THROW(Err::CriticalError, TX("Device not initialized."));
		}

		LPDIRECT3DTEXTURE9 texture;

		if (m_device->CreateTexture(dimensions.Width, dimensions.Height, 0, D3DUSAGE_RENDERTARGET, fmt, D3DPOOL_DEFAULT, &texture, 0) != D3D_OK) {
			DO_THROW(Err::Direct3DError, TX("Couldn't allocate texture."));
		}

		return Texture::Ptr(new Texture(texture));
	}

	VertexBuffer::Ptr Device::CreateVertexBuffer(size_t sizeInBytes, int fmt) {
		if (m_device == 0) {
			DO_THROW(Err::CriticalError, TX("Device not initialized."));
		}

		LPDIRECT3DVERTEXBUFFER9 vb;
		if (D3D_OK != m_device->CreateVertexBuffer(sizeInBytes, D3DUSAGE_WRITEONLY, fmt, D3DPOOL_DEFAULT, &vb, 0)) {
			DO_THROW(Err::CriticalError, TX("Couldn't create vertex buffer"));
		}

		return VertexBuffer::Ptr(new VertexBuffer(vb, fmt, sizeInBytes));
	}

	void Device::SetTexture(int stage, Texture::Ptr texture) {
		if (m_device == 0) {
			DO_THROW(Err::CriticalError, TX("Device not initialized."));
		}
		if (texture == 0) {
			DO_THROW(Err::InvalidParam, TX("texture was null."));
		}

		m_device->SetTexture(stage, texture->D3DObject());
	}

	void Device::SetSwapChain(SwapChain::Ptr swapChain) {
		if (m_device == 0) {
			DO_THROW(Err::CriticalError, TX("Device not initialized."));
		}
		if (swapChain == 0){
			DO_THROW(Err::InvalidParam, TX("Null pointer not really valid."));
		}
		m_currentSwapChain = swapChain;

		LPDIRECT3DSWAPCHAIN9 sc = m_currentSwapChain->D3DObject();
		if (sc == 0) {
			DO_THROW(Err::CriticalError, TX("Swap chain was not initialized."));
		}

		LPDIRECT3DSURFACE9 backBuffer = 0;
		if (sc->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &backBuffer) != D3D_OK) {
			DO_THROW(Err::Direct3DError, TX("Couldn't get back buffer from swap chain."));
		}
		if (m_device->SetRenderTarget(0, backBuffer) != D3D_OK) {
			DO_THROW(Err::Direct3DError, TX("Couldn't set render target."));
		}
		backBuffer->Release();
	}

	void Device::ResampleFilter(DWORD stage, D3DTEXTUREFILTERTYPE filter) {
		HRESULT hret = m_device->SetSamplerState(stage, D3DSAMP_MINFILTER, filter);
		if (hret != D3D_OK) {
			DO_THROW(Err::Direct3DError, TX("SetSamplerState(D3DSAMP_MINFILTER) failed"));
		}
		hret = m_device->SetSamplerState(stage, D3DSAMP_MAGFILTER, filter);
		if (hret != D3D_OK) {
			DO_THROW(Err::Direct3DError, TX("SetSamplerState(D3DSAMP_MAGFILTER) failed"));
		}
	}

	void Device::BeginDraw() {
		if (m_device == 0) {
			DO_THROW(Err::CriticalError, TX("Device not initialized."));
		}
		if (m_isDrawing) {
			DO_THROW(Err::CriticalError, TX("Device was already drawing."));
		}
		if (m_device->BeginScene() != D3D_OK) {
			DO_THROW(Err::Direct3DError, TX("BeginScene call failed."));
		}

		if (D3D_OK != m_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE)) {
			DO_THROW(Err::Direct3DError, TX("SetRenderState(D3DRS_CULLMODE) failed"));
		}
		if (D3D_OK != m_device->SetRenderState(D3DRS_LIGHTING, false)) {
			DO_THROW(Err::Direct3DError, TX("SetRenderState(D3DRS_LIGHTING) failed"));
		}

		D3DMATRIX mat;
		ZeroMemory(&mat, sizeof(mat));
		mat._11 = 1;
		mat._22 = 1;
		mat._33 = 1;
		mat._44 = 1;
		SetMatrix(D3DTS_WORLD, &mat);
		SetMatrix(D3DTS_VIEW, &mat);

		m_isDrawing = true;
	}

#define CONVERT_VERTEX2D_POS(v) v.Position.X, v.Position.Y, 0
#define CONVERT_VERTEX2D_TEX1(v) v.TexCoord.X, v.TexCoord.Y
#define CONVERT_VERTEX2D_VBVERTEX(v) CONVERT_VERTEX2D_POS(v), CONVERT_VERTEX2D_TEX1(v)

	void Device::RenderQuad(const Vertex2D& a, const Vertex2D& b, const Vertex2D& c, const Vertex2D& d) {
		if (m_isDrawing == false) DO_THROW(Err::CriticalError, TX("Device was not drawing."));

		const vbVertex c_vertices[] = {
			{ CONVERT_VERTEX2D_VBVERTEX(a) },
			{ CONVERT_VERTEX2D_VBVERTEX(b) },
			{ CONVERT_VERTEX2D_VBVERTEX(c) },
			{ CONVERT_VERTEX2D_VBVERTEX(d) }
		};
		m_device->SetFVF(D3D_VERTEX_FVF);
		m_device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, c_vertices, sizeof(vbVertex));
	}

	void Device::RenderQuad( int index ) {
		if (D3D_OK != m_device->DrawPrimitive(D3DPT_TRIANGLESTRIP, index * 4, 2)) {
			DO_THROW(Err::Direct3DError, TX("Failed to render quad."));
		}
	}

	void Device::EndDraw() {
		if (m_device == 0) {
			DO_THROW(Err::CriticalError, TX("Device not initialized."));
		}
		if (m_isDrawing == false) {
			DO_THROW(Err::CriticalError, TX("Device was not drawing."));
		}

		m_device->EndScene();
		m_isDrawing = false;
	}

	void Device::SetMatrix(D3DTRANSFORMSTATETYPE state, CONST D3DMATRIX * matrix) {
		if (matrix == 0) {
			DO_THROW(Err::InvalidParam, TX("matrix was null."));
		}
		if (m_device == 0) {
			DO_THROW(Err::CriticalError, TX("Device not initialized."));
		}
		m_device->SetTransform(state, matrix);
	}

	Device::Device():
		m_device{ 0 },
		m_isDrawing{ false },
		m_d3d{ RequestDirect3D() }
	{}

	Device::~Device() {
		Release();
		//DropDirect3D();
	}

	void Device::SetRenderTarget(Texture::Ptr renderTarget) {
		LPDIRECT3DSURFACE9 surface;
		renderTarget->D3DObject()->GetSurfaceLevel(0, &surface);
		if (m_device->SetRenderTarget(0, surface) != D3D_OK) {
			DO_THROW(Err::Direct3DError, TX("Couldn't set render target."));
		}
		surface->Release();
	}

	const Geom::SizeInt Device::MaxTextureSize() const {
		D3DCAPS9 caps;
		m_device->GetDeviceCaps(&caps);
		return Geom::SizeInt(caps.MaxTextureWidth, caps.MaxTextureHeight);
	}

	void Device::SendTextureRect( D3D::Texture::Ptr sourceTexture, const Geom::RectInt& sourceRect, D3D::Texture::Ptr destinationTexture, const Geom::PointInt& destinationTopLeft ) {
		LPDIRECT3DSURFACE9 sourceSurface = 0;
		sourceTexture->D3DObject()->GetSurfaceLevel(0, &sourceSurface);

		LPDIRECT3DSURFACE9 destinationSurface = 0;
		destinationTexture->D3DObject()->GetSurfaceLevel(0, &destinationSurface);

		RECT winapi_sourceRect = RectToRECT(sourceRect);
		POINT winapi_destPoint = PointToPOINT(destinationTopLeft);

		m_device->UpdateSurface(sourceSurface, &winapi_sourceRect, destinationSurface, &winapi_destPoint);

		sourceSurface->Release();
		destinationSurface->Release();
	}

	void Device::Present() {
		m_device->Present(0, 0, 0, 0);
	}

	bool Device::IsLost() {
		return (m_device->TestCooperativeLevel() != D3D_OK);
	}

	bool Device::Reset() {
		m_currentSwapChain.reset();
		HRESULT hret = m_device->Reset(&m_presentParams);
		return (hret == D3D_OK);
	}

	void Device::Release() {
		m_currentSwapChain.reset();
		SAFE_RELEASE(m_device);
	}

	void Device::SetVertexBuffer( VertexBuffer::Ptr vb, int stride ) {
		m_device->SetFVF(vb->FVFDefinition());
		m_device->SetStreamSource(0, vb->D3DObject(), 0, stride);
	}

#pragma warning (disable: 4715)
	D3DFORMAT ToD3DFORMAT(Img::Format format) {
		switch(format) {
			case Img::Format::ARGB8888:
				return D3DFMT_A8R8G8B8;
			case Img::Format::XRGB8888:
				return D3DFMT_X8R8G8B8;
			case Img::Format::RGB565:
				return D3DFMT_R5G6B5;
			case Img::Format::ARGB1555:
				return D3DFMT_A1R5G5B5;
			case Img::Format::XRGB1555:
				return D3DFMT_X1R5G5B5;
			case Img::Format::Index8:
				return D3DFMT_L8;
			default:
				DO_THROW(Err::InvalidParam, TX("Unsupported texture format: ") + ToWString(format));
		}
	}
}
