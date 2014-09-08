#ifndef SURFACED3D_H
#define SURFACED3D_H

#include <windows.h>

#include <vector>

#include <boost/shared_array.hpp>

#include <illa/types.h>
#include <illa/surfacefactory.h>

#include "d3d_device.h"
#include "w32_surface.h"

namespace Win
{
	using boost::shared_array;
	using std::vector;

	/**
	* Surface implemented with Direct3D.
	*/
	class SurfaceD3D:public WinSurface
	{
	public:
		void						ExplicitLock();
		void						ExplicitUnlock();

		D3D::Texture::Ptr			PaletteTexture();

		SurfaceD3D(D3D::Device::Ptr);
		~SurfaceD3D();

		struct SourceTexture
		{
			Geom::RectInt			LocalAreaCovered;
			Geom::RectInt			GlobalAreaCovered;
			D3D::Texture::Ptr		Texture;
			size_t					Index;
		};

		typedef vector<SourceTexture> SourceTextureList;
		SourceTextureList			QueryAffectedTextures(const Geom::RectInt& region);

	private:
		bool						onFinalizeSurface();

		bool						onCreateSurface();

		Geom::SizeInt				calcNumTextures( const Geom::SizeInt& maxSize );
		unsigned char*				onLockSurface(const Geom::RectInt& region, Img::LockMethod method = Img::LockReadWrite);

		void transferBufferData( Byte* destination, size_t destinationStride, Byte* source, size_t sourceStride, size_t bytesPerScanline, size_t numScanlines);
		void						onUnlockSurface();
		void						onPaletteUpdate();

		size_t						onStride() const;

		void						onDrawAreaToBuffer(Byte* pDest, int strideDest, const Geom::RectInt& sourceRect, const Img::Color& bgrColor, int brightness, int contrast, int gamma);

	private:
		typedef vector<D3D::Texture::Ptr> TextureVector;
		typedef shared_array<Byte>	ByteArray;

		Geom::RectInt				globalToLocalRegion(size_t textureIndex, const Geom::RectInt& globalRect);

		void						copyTextureToBuffer(D3D::Texture::Ptr sourceTexture, const Geom::RectInt& localTextureArea, const Geom::PointInt& destinationTopLeft, const Geom::RectInt& bufferArea, Byte* buffer);
		void						copyBufferToTexture(D3D::Texture::Ptr destinationTexture, const Geom::PointInt& destinationTopLeft, const Geom::RectInt& bufferAreaToCopy, const Geom::RectInt& bufferArea, Byte* buffer);

	private:
		Img::LockStrategy::Ptr		m_lockStrategy;
		D3D::Device::Ptr			m_direct3d;
		D3D::Texture::Ptr			m_texPalette;

		TextureVector				m_textures;
		SourceTextureList			m_lockedTextures;
		Geom::RectInt				m_lockedRegion;

		ByteArray					m_lockedArea;

		size_t						m_pitch;
		bool						m_isReadOnlyLock;
	};
}

#endif // SURFACED3D_H
