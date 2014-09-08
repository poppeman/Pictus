
#include "surfaced3d.h"

#include <illa/surface_lock_exclusive.h>

namespace Win
{
	using Geom::PointInt;
	using Geom::SizeInt;
	using Geom::RectInt;

	SurfaceD3D::SurfaceD3D(D3D::Device::Ptr device)
		:m_direct3d(device),
		 m_lockStrategy(Img::LockStrategy::Ptr(new Img::LockStrategyExclusive))
	{
		SetLockStrategy(m_lockStrategy);
	}

	SurfaceD3D::~SurfaceD3D() {}

	bool SurfaceD3D::onCreateSurface()
	{
		COND_STRICT(m_textures.size() == 0, Err::CriticalError, TX("Tried to re-allocate surface."));

		SizeInt maxSize = m_direct3d->MaxTextureSize();
		SizeInt numTextures = calcNumTextures(maxSize);

		COND_STRICT(Area(numTextures) > 0, Err::CriticalError, TX("Wanted to create zero textures, this is pretty wrong."));

		m_textures.reserve(Area(numTextures));
		for (int y = 0; y < numTextures.Height; ++y)
		{
			for (int x = 0; x < numTextures.Width; ++x)
			{
				SizeInt currTextureSize = Util::Min(maxSize, GetSize() - maxSize * SizeInt(x, y));
				m_textures.push_back(m_direct3d->CreateTexture(currTextureSize, D3D::ToD3DFORMAT(GetFormat()), D3DPOOL_SYSTEMMEM));
			}
		}

		if (GetFormat() == Img::FormatIndex8)
		{
			m_texPalette = m_direct3d->CreateTexture(SizeInt(256, 1), D3DFMT_X8R8G8B8, D3DPOOL_MANAGED);
			if (m_texPalette == 0) return false;
		}

		return true;
	}

	unsigned char* SurfaceD3D::onLockSurface(const RectInt& region, Img::LockMethod method)
	{
		COND_STRICT(m_lockedTextures.size() == 0, Err::CriticalError, TX("Attempted to re-lock an already locked surface"));
		COND_STRICT(m_textures.size() != 0, Err::CriticalError, TX("Surface not properly initialized."));
		m_lockedTextures = QueryAffectedTextures(region);

		COND_STRICT(m_lockedTextures.size() != 0, Err::CriticalError, TX("No textures matched the requested region."));
		COND_WARNING(m_lockedTextures.size() <= m_textures.size(), Err::CriticalError, TX("Attempting to lock more textures than that actually exist."));

		// Easier and faster to just use the buffer directly (IFF the lock rect is covered by a single texture)
		if (m_lockedTextures.size() == 1)
		{
			D3D::Texture::Ptr textureToUse = m_textures[m_lockedTextures[0].Index];

			Byte* buffer = textureToUse->Lock(globalToLocalRegion(0, region), (method == Img::LockRead));
			m_pitch = textureToUse->Pitch();
			return buffer;
		}

		m_pitch = PixelSize() * region.Width();
		m_lockedRegion = region;
		m_lockedArea.reset(new Byte[PixelSize() * Area(region.Dimensions())]);

		for (SourceTextureList::iterator i = m_lockedTextures.begin(); i != m_lockedTextures.end(); ++i)
		{
			copyTextureToBuffer(i->Texture, i->LocalAreaCovered, i->GlobalAreaCovered.TopLeft(), region, m_lockedArea.get());
		}

		return m_lockedArea.get();
	}

	void SurfaceD3D::onUnlockSurface()
	{
		if (m_lockedTextures.size() == 1)
		{
			m_lockedTextures[0].Texture->Unlock();
		}
		else
		{
			for (SourceTextureList::iterator i = m_lockedTextures.begin(); i != m_lockedTextures.end(); ++i)
			{
				copyBufferToTexture(i->Texture, i->LocalAreaCovered.TopLeft(), i->GlobalAreaCovered, m_lockedRegion, m_lockedArea.get());
				/*Byte* buffer = i->Texture->Lock(i->LocalAreaCovered, Img::LockReadWrite);

				size_t tempPitch		= m_pitch;
				size_t bytesToCopy		= i->LocalAreaCovered.Width() * PixelSize();
				size_t texturePitch		= i->Texture->Pitch();
				Byte* currTempScanline	= 
					m_lockedArea.get() + 
					(i->GlobalAreaCovered.Left() - m_lockedRegion.Left()) * PixelSize() +
					(i->GlobalAreaCovered.Top() - m_lockedRegion.Top()) * tempPitch;

				transferBufferData(buffer, texturePitch, currTempScanline, tempPitch, bytesToCopy, i->LocalAreaCovered.Height());*/
			}
		}
		m_lockedTextures.clear();
		/*COND_WARNING(m_texMap, Err::CriticalError, TX("Texture was not created."));
		m_texMap->Unlock();*/
	}

	void SurfaceD3D::onPaletteUpdate()
	{
		COND_STRICT(m_texPalette, Err::CriticalError, TX("Palette look-up texture not created."));
		DWord* data = reinterpret_cast<DWord*>(m_texPalette->Lock(RectInt(PointInt(0, 0), SizeInt(256, 1)), false));

		for (int i = 0; i < 256; ++i)
		{
			data[i] = ToARGBDWORD(GetPalette().Color(i));
		}
		m_texPalette->Unlock();
	}

	bool SurfaceD3D::onFinalizeSurface()
	{
		return true;
	}

	void SurfaceD3D::onDrawAreaToBuffer(Byte*, int, const RectInt&, const Img::Color&, int, int, int) {}

	size_t SurfaceD3D::onStride() const
	{
		return m_pitch;
	}

	/*D3D::Texture::Ptr SurfaceD3D::NativeTexture()
	{
		//return m_texMap;
		return m_textures.at(0);
	}*/

	D3D::Texture::Ptr SurfaceD3D::PaletteTexture()
	{
		return m_texPalette;
	}

	Geom::RectInt SurfaceD3D::globalToLocalRegion( size_t textureIndex, const Geom::RectInt& globalRect )
	{
		// TODO: Implement

		SizeInt maxSize		= m_direct3d->MaxTextureSize();
		PointInt coordinate	= globalRect.TopLeft() / maxSize;

		PointInt textureTopLeft = coordinate * maxSize;

		RectInt textureFullArea(
			textureTopLeft,
			Util::Min(globalRect.BottomRight(), textureTopLeft + maxSize));

		// FIXME: Crop global rect to the full texture rect

		// Adjust to local coords
		SizeInt adjustmentAmount = (PointInt(0, 0) - textureFullArea.TopLeft());
		return RectInt(
			(globalRect.TopLeft() + adjustmentAmount ),
			globalRect.Dimensions());


		/*return Geom::RectInt(
			Util::Max(globalRect.TopLeft(), PointInt(x * maxSize.Width, y * maxSize.Height)),
			Util::Min(globalRect.BottomRight(), PointInt((x + 1) * maxSize.Width, (y + 1) * maxSize.Height)));*/


		//SizeInt numTextures = calcNumTextures(maxSize);

		//return globalRect;
		//return Geom::RectInt(PointInt(0, 0), globalRect.Dimensions());
		return Geom::RectInt(PointInt(globalRect.Left() % maxSize.Width, globalRect.Top() & maxSize.Height), globalRect.Dimensions());
	}

	SurfaceD3D::SourceTextureList SurfaceD3D::QueryAffectedTextures( const Geom::RectInt& region )
	{
		SourceTextureList outList;

		SizeInt maxSize = m_direct3d->MaxTextureSize();

		int left = region.Left() / maxSize.Width;
		int right = (region.Right() + maxSize.Width - 1) / maxSize.Width;
		int top = region.Top() / maxSize.Height;
		int bottom = (region.Bottom() + maxSize.Height - 1) / maxSize.Height;

		SizeInt numTextures = calcNumTextures(maxSize);

		for (int y = top; y < bottom; y++)
		{
			for (int x = left; x < right; x++)
			{
				SourceTexture currentTexture;

				currentTexture.Index = x + numTextures.Width * y;

				currentTexture.GlobalAreaCovered = RectInt(
					Util::Max(region.TopLeft(), PointInt(x * maxSize.Width, y * maxSize.Height)),
					Util::Min(region.BottomRight(), PointInt((x + 1) * maxSize.Width, (y + 1) * maxSize.Height)));
				currentTexture.LocalAreaCovered = globalToLocalRegion(currentTexture.Index, currentTexture.GlobalAreaCovered);

				currentTexture.Texture = m_textures[currentTexture.Index];
				outList.push_back(currentTexture);
			}
		}
		return outList;
	}

	void SurfaceD3D::ExplicitLock()
	{
		m_lockStrategy->AquireLock(Img::LockReadWrite);
	}

	void SurfaceD3D::ExplicitUnlock()
	{
		m_lockStrategy->ReleaseLock(Img::LockReadWrite);
	}

	Win::SizeInt SurfaceD3D::calcNumTextures( const SizeInt& maxSize )
	{
		return (GetSize() + maxSize - SizeInt(1, 1)) / maxSize;
	}

	void SurfaceD3D::transferBufferData( Byte* destination, size_t destinationStride, Byte* source, size_t sourceStride, size_t bytesPerScanline, size_t numScanlines )
	{
		for (int y = 0; y < numScanlines; ++y)
		{
			memcpy(destination, source, bytesPerScanline);
			destination	+= destinationStride;
			source		+= sourceStride;
		}
	}

	void SurfaceD3D::copyTextureToBuffer( D3D::Texture::Ptr sourceTexture, const Geom::RectInt& localTextureArea, const Geom::PointInt& destinationTopLeft, const Geom::RectInt& bufferArea, Byte* buffer )
	{
		Byte* textureData = sourceTexture->Lock(localTextureArea, Img::LockRead);
		COND_STRICT(textureData, Err::CriticalError, TX("Couldn't lock texture."));

		size_t bufferPitch		= bufferArea.Width() * PixelSize();
		size_t bytesToCopy		= localTextureArea.Width() * PixelSize();
		size_t texturePitch		= sourceTexture->Pitch();
		
		buffer += 
			(destinationTopLeft.X - bufferArea.Left()) * PixelSize() +
			(destinationTopLeft.Y - bufferArea.Top()) * bufferPitch;

		transferBufferData(buffer, bufferPitch, textureData, texturePitch, bytesToCopy, localTextureArea.Height());

		sourceTexture->Unlock();
	}

	void SurfaceD3D::copyBufferToTexture( D3D::Texture::Ptr destinationTexture, const Geom::PointInt& destinationTopLeft, const Geom::RectInt& bufferAreaToCopy, const Geom::RectInt& bufferArea, Byte* buffer )
	{
		Byte* textureData = destinationTexture->Lock(RectInt(destinationTopLeft, bufferAreaToCopy.Dimensions()), Img::LockReadWrite);
		COND_STRICT(textureData, Err::CriticalError, TX("Couldn't lock texture."));

		size_t bufferPitch		= bufferArea.Width() * PixelSize();
		size_t bytesToCopy		= bufferAreaToCopy.Width() * PixelSize();
		size_t texturePitch		= destinationTexture->Pitch();

		buffer += 
			(bufferAreaToCopy.Left() - bufferArea.Left()) * PixelSize() +
			(bufferAreaToCopy.Top() - bufferArea.Top()) * bufferPitch;

		transferBufferData(textureData, texturePitch, buffer, bufferPitch, bytesToCopy, bufferAreaToCopy.Height());

		destinationTexture->Unlock();
	}
}
