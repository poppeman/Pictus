#include "d3d_device.h"
#include "d3d_common.h"
#include "int_d3d_types.h"
#include <memory>

namespace D3D {
	using Geom::SizeInt;

	template<typename T> ::std::shared_ptr<T> WrapRelease(T* obj) {
		return ::std::shared_ptr<T>( obj, ComRelease<T>);
	}

	bool Device::Initialize(HWND hwndFocus, bool doWindowed, int device) {
		// Can occur of Direct3D9 is not installed.
		if (m_d3d == 0) throw Err::Direct3DError("Direct3D 9 could not be found");

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
		IDirect3DDevice9Ex* pdev = nullptr;
		auto hr = m_d3d->CreateDeviceEx(
			device,
			D3DDEVTYPE_HAL,
			hwndFocus,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&m_presentParams,
			nullptr,
			&pdev);

		switch (hr) {
		case D3DERR_DEVICELOST:
			throw Err::Direct3DError("Direct3D Device was lost.");
		case D3DERR_INVALIDCALL:
			throw Err::Direct3DError("Invalid call during CreateDevice.");
		case D3DERR_NOTAVAILABLE:
			throw Err::Direct3DError("Direct3D device does not support required.");
		case D3DERR_OUTOFVIDEOMEMORY:
			throw Err::Direct3DError("Not enough video memory to initialize Direct3D.");
		}

		m_device = WrapRelease(pdev);

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

	void Device::ResizeBackBuffer(const Geom::SizeInt& newSize) {
		m_presentParams.BackBufferWidth = newSize.Width;
		m_presentParams.BackBufferHeight = newSize.Height;

		m_device->ResetEx(&m_presentParams, nullptr);
	}

	Geom::SizeInt Device::BackBufferSize() const {
		IDirect3DSurface9* pbuf;
		m_device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pbuf);
		D3DSURFACE_DESC desc;
		pbuf->GetDesc(&desc);
		pbuf->Release();
		return Geom::SizeInt{ static_cast<int>(desc.Width), static_cast<int>(desc.Height) };
	}



	void Device::Clear( int a, int r, int g, int b ) {
		m_device->Clear(0, 0, D3DCLEAR_TARGET, D3DCOLOR_ARGB(a, r, g, b), 0, 0);
	}

	Texture::Ptr Device::CreateTexture(const Geom::SizeInt& dimensions, D3DFORMAT fmt, D3DPOOL pool) {
		if (m_device == 0) {
			DO_THROW(Err::CriticalError, "Device not initialized.");
		}
		LPDIRECT3DTEXTURE9 texture;

		if (m_device->CreateTexture(dimensions.Width, dimensions.Height, 1, 0, fmt, pool, &texture, 0) != D3D_OK) {
			DO_THROW(Err::Direct3DError, "Couldn't allocate texture.");
		}

		return Texture::Ptr(new Texture(texture));
	}

	Texture::Ptr Device::CreateRenderTarget(const Geom::SizeInt& dimensions, D3DFORMAT fmt) {
		if (m_device == 0) {
			DO_THROW(Err::CriticalError, "Device not initialized.");
		}

		LPDIRECT3DTEXTURE9 texture;

		if (m_device->CreateTexture(dimensions.Width, dimensions.Height, 0, D3DUSAGE_RENDERTARGET, fmt, D3DPOOL_DEFAULT, &texture, 0) != D3D_OK) {
			DO_THROW(Err::Direct3DError, "Couldn't allocate texture.");
		}

		return Texture::Ptr(new Texture(texture));
	}

	VertexBuffer::Ptr Device::CreateVertexBuffer(size_t sizeInBytes, int fmt) {
		if (m_device == 0) {
			DO_THROW(Err::CriticalError, "Device not initialized.");
		}

		LPDIRECT3DVERTEXBUFFER9 vb;
		if (D3D_OK != m_device->CreateVertexBuffer(sizeInBytes, D3DUSAGE_WRITEONLY, fmt, D3DPOOL_DEFAULT, &vb, 0)) {
			DO_THROW(Err::CriticalError, "Couldn't create vertex buffer");
		}

		return VertexBuffer::Ptr(new VertexBuffer(vb, fmt, sizeInBytes));
	}

	void Device::SetTexture(int stage, Texture::Ptr texture) {
		if (m_device == 0) {
			DO_THROW(Err::CriticalError, "Device not initialized.");
		}
		if (texture == 0) {
			DO_THROW(Err::InvalidParam, "texture was null.");
		}

		m_device->SetTexture(stage, texture->D3DObject());
	}

	void Device::ResampleFilter(DWORD stage, D3DTEXTUREFILTERTYPE filter) {
		HRESULT hret = m_device->SetSamplerState(stage, D3DSAMP_MINFILTER, filter);
		if (hret != D3D_OK) {
			DO_THROW(Err::Direct3DError, "SetSamplerState(D3DSAMP_MINFILTER) failed");
		}
		hret = m_device->SetSamplerState(stage, D3DSAMP_MAGFILTER, filter);
		if (hret != D3D_OK) {
			DO_THROW(Err::Direct3DError, "SetSamplerState(D3DSAMP_MAGFILTER) failed");
		}
	}

	void Device::BeginDraw() {
		if (m_device == 0) {
			DO_THROW(Err::CriticalError, "Device not initialized.");
		}
		if (m_isDrawing) {
			DO_THROW(Err::CriticalError, "Device was already drawing.");
		}
		if (m_device->BeginScene() != D3D_OK) {
			DO_THROW(Err::Direct3DError, "BeginScene call failed.");
		}

		if (D3D_OK != m_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE)) {
			DO_THROW(Err::Direct3DError, "SetRenderState(D3DRS_CULLMODE) failed");
		}
		if (D3D_OK != m_device->SetRenderState(D3DRS_LIGHTING, false)) {
			DO_THROW(Err::Direct3DError, "SetRenderState(D3DRS_LIGHTING) failed");
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
		if (m_isDrawing == false) DO_THROW(Err::CriticalError, "Device was not drawing.");

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
			DO_THROW(Err::Direct3DError, "Failed to render quad.");
		}
	}

	void Device::EndDraw() {
		if (m_device == nullptr) {
			DO_THROW(Err::CriticalError, "Device not initialized.");
		}
		if (m_isDrawing == false) {
			DO_THROW(Err::CriticalError, "Device was not drawing.");
		}

		m_device->EndScene();
		m_device->PresentEx(0, 0, 0, 0, 0);

		m_isDrawing = false;
	}

	void Device::SetMatrix(D3DTRANSFORMSTATETYPE state, CONST D3DMATRIX * matrix) {
		if (matrix == 0) {
			DO_THROW(Err::InvalidParam, "matrix was null.");
		}
		if (m_device == 0) {
			DO_THROW(Err::CriticalError, "Device not initialized.");
		}
		m_device->SetTransform(state, matrix);
	}

	Device::Device():
		m_device{ 0 },
		m_isDrawing{ false }
	{
		IDirect3D9Ex* pdex = 0;
		if (FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &pdex))) {
			pdex = 0;
		}

		m_d3d = WrapRelease(pdex);
	}

	Device::~Device() {
		Release();
	}

	void Device::SetRenderTarget(Texture::Ptr renderTarget) {
		LPDIRECT3DSURFACE9 surface;
		renderTarget->D3DObject()->GetSurfaceLevel(0, &surface);
		if (m_device->SetRenderTarget(0, surface) != D3D_OK) {
			DO_THROW(Err::Direct3DError, "Couldn't set render target.");
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
		m_device->PresentEx(0, 0, 0, 0, 0);
	}

	bool Device::IsLost() {
		return (m_device->TestCooperativeLevel() != D3D_OK);
	}

	bool Device::Reset() {
		HRESULT hret = m_device->ResetEx(&m_presentParams, nullptr);
		return (hret == D3D_OK);
	}

	void Device::Release() {
		m_device = nullptr;
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
				DO_THROW(Err::InvalidParam, "Unsupported texture format: " + ToAString(format));
		}
	}
}
