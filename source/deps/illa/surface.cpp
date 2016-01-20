#include "surface.h"
#include "surface_locked_area.h"
#include "filter.h"
#include "config.h"
#include "orz/exception.h"
#include "orz/logger.h"
#include "orz/types.h"
#include <cstring>

namespace Img {
	using Geom::PointInt;
	using Geom::SizeInt;
	using Geom::RectInt;

	void Surface::CreateSurface(const SizeInt& size, Format format) {
		if (m_isCreated) {
			DO_THROW(Err::CriticalError, "Surface already created.");
		}

		// Sanity check
		if ((size.Width <= 0) || (size.Height <= 0)) {
			DO_THROW(Err::InvalidParam, "Invalid dimensions, both axis must be above zero");
		}

		if (size.Width > MaxSurfaceDim || size.Height > MaxSurfaceDim)
		{
			Log << "(Surface::CreateSurface) Too large surface requested, maximum axis size is " << ToAString(MaxSurfaceDim) << "\n";
			throw std::bad_alloc();
		}
		if (format >= Format::Num) {
			DO_THROW(Err::InvalidParam, "Invalid format");
		}
		auto bytesToConsume = static_cast<size_t>(size.Width) * static_cast<size_t>(size.Height) * static_cast<size_t>(EstimatePixelSize(format));
		if (bytesToConsume > MaxSurfaceBytes) {
			Log << "Surface would consume " << ToAString(bytesToConsume) << " which would be greater than the safety limit of " << ToAString(MaxSurfaceBytes) << " bytes\n";
			throw std::bad_alloc();
		}

		m_swFormat	= format;
		m_size		= size;
		m_pixelSize	= EstimatePixelSize(m_swFormat);

		ForceDirty();

		onCreateSurface();
		m_isCreated = true;
	}

	bool Surface::CopyRectToBuffer(uint8_t* pDest, int strideDest, const RectInt& sourceRect, const Img::Color& bgrColor, int brightness, int contrast, int gamma, bool applyAlpha) {
		onCopyAreaToBuffer(pDest, strideDest, sourceRect, bgrColor, brightness, contrast, gamma, applyAlpha);
		return true;
	}

	void Surface::ClearSurface(const Img::Color& col) {
		onClearSurface(col);
	}

	void Surface::CopySurface(Surface::Ptr source) {
		CopySurface(source, PointInt(0, 0));
	}

	void Surface::CopySurface(Surface::Ptr source, PointInt destinationTopLeft) {
		CopySurface(source, RectInt(PointInt(0, 0), source->GetSize()), destinationTopLeft);
	}

	void Surface::CopySurface(Surface::Ptr source, RectInt sourceAreaToCopy, PointInt destinationTopLeft) {
		onCopySurface(source, sourceAreaToCopy, destinationTopLeft);
	}

	void Surface::BlitSurfaceAlpha(Surface::Ptr source, Geom::PointInt destinationTopLeft)
	{
		BlitSurfaceAlpha(source, RectInt{ PointInt{0, 0}, source->GetSize() }, destinationTopLeft);
	}

	void Surface::BlitSurfaceAlpha(Surface::Ptr source, Geom::RectInt sourceAreaToCopy, Geom::PointInt destinationTopLeft)
	{
		onBlitSurfaceAlpha(source, sourceAreaToCopy, destinationTopLeft);
	}

	void Surface::BlitSurfaceColorKey(Surface::Ptr source, PointInt destinationTopLeft, uint8_t colorKeyIndex) {
		BlitSurfaceColorKey(source, RectInt(PointInt(0, 0), source->GetSize()), destinationTopLeft, colorKeyIndex);
	}

	void Surface::BlitSurfaceColorKey( Surface::Ptr source, RectInt sourceAreaToCopy, PointInt destinationTopLeft, uint8_t colorKeyIndex ) {
		if (source->GetFormat() != Format::Index8) {
			DO_THROW(Err::InvalidParam, "Source buffer must be indexed.");
		}

		onBlitSurfaceColorKey(source, sourceAreaToCopy, destinationTopLeft, colorKeyIndex);
	}

	Surface::LockedArea::Ptr Surface::LockSurface(const RectInt& region, LockMethod method) {
		m_strategyLocking->AquireLock(method);
		try {
			if (!RectInt(PointInt(0, 0), GetSize()).Contains(region)) {
				throw Err::InvalidParam("Requested region was outside the bounds of the surface");
			}

			uint8_t* pData = onLockSurface(region, method);
			if (pData == nullptr)
			{
				// This can happen if ->CreateSurface have not yet been called or is not finished calling
				return nullptr;
			}

			if (method == LockReadWrite) {
				ForceDirty();
			}

			return std::make_shared<LockedArea>(this, pData, region, method, onStride());
		}
		catch (...) {
			m_strategyLocking->ReleaseLock(method);
			throw;
		}
	}

	Surface::LockedArea::Ptr Surface::LockSurface(LockMethod method) {
		return LockSurface(RectInt(PointInt(0,0), GetSize()), method);
	}

	void Surface::UnlockSurface(LockMethod method) {
		onUnlockSurface();

		m_strategyLocking->ReleaseLock(method);
	}

	void Surface::onClearSurface(const Img::Color& col) {
		FillRect(RectInt(PointInt(0, 0), GetSize()), col);
	}

	size_t Surface::onStride() const {
		return Width() * m_pixelSize;
	}

	Surface::Surface():
		m_pixelSize{ 0 },
		m_isDirty{ true },
		m_swFormat{ Format::Undefined },
		m_isCreated{ false }
	{
		
	}

	Surface::~Surface() {}

	bool Surface::IsDirty() {
		std::lock_guard<std::mutex> l(m_mutexDirty);
		bool currDirty = m_isDirty;
		m_isDirty = false;
		return currDirty;
	}

	void Surface::SetPalette(const Img::Palette& pal) {
		m_swPal = pal;
		onPaletteUpdate();
	}

	Img::Palette Surface::GetPalette() {
		return m_swPal;
	}

	void Surface::ForceDirty() {
		std::lock_guard<std::mutex> l(m_mutexDirty);
		m_isDirty = true;
	}

	const SizeInt& Surface::GetSize() const {
		return m_size;
	}

	int Surface::PixelSize() const {
		return m_pixelSize;
	}

	Img::Format Surface::GetFormat() const {
		return m_swFormat;
	}

	void Surface::SetLockStrategy(LockStrategy::Ptr strategy) {
		m_strategyLocking = strategy;
	}

	void Surface::onCopySurface(Surface::Ptr source, RectInt sourceAreaToCopy, PointInt destinationTopLeft) {
		if (source->GetFormat() == GetFormat()) {
			copyNoConversion(source, sourceAreaToCopy, destinationTopLeft);
		}
		else if (GetFormat() == Img::Format::ARGB8888 || GetFormat() == Img::Format::XRGB8888) {
			copyConvertAnyTo32(source, sourceAreaToCopy, destinationTopLeft);
		}
	}

	void Surface::onBlitSurfaceAlpha(Surface::Ptr source, Geom::RectInt sourceAreaToCopy, Geom::PointInt destinationTopLeft)
	{
		if (GetFormat() != Format::ARGB8888)
		{
			DO_THROW(Err::InvalidParam, "onBlitSurfaceAlpha dest-type != ARGB8888 Not yet supported");
		}

		
		LockedArea::Ptr lockedSource = source->LockSurface();
		LockedArea::Ptr lockedDestination = LockSurface();

		Filter::FilterBuffer src(
			source->GetSize(),
			source->PixelSize(),
			lockedSource->Buffer(),
			lockedSource->Stride(),
			source->GetPalette());
		Filter::FilterBuffer dst(
			GetSize(),
			PixelSize(),
			lockedDestination->Buffer(),
			lockedDestination->Stride());

		Filter::Alpha::Crossblend(src, sourceAreaToCopy.TopLeft(), dst, destinationTopLeft, sourceAreaToCopy.Dimensions(), dst, destinationTopLeft);
	}

	void Surface::copyNoConversion(Surface::Ptr source, RectInt &sourceAreaToCopy, PointInt destinationTopLeft) {
		LockedArea::Ptr lockedSource = source->LockSurface(sourceAreaToCopy, LockRead);
		uint8_t* pOldDest = lockedSource->Buffer();

		LockedArea::Ptr lockedDestination = LockSurface(RectInt(destinationTopLeft, sourceAreaToCopy.Dimensions()));
		uint8_t* pNewDest = lockedDestination->Buffer();

		int scans	= std::min(source->Height(), Height());
		int width	= std::min(source->Width(), Width());
		for (int i = 0; i < scans; ++i) {
			memcpy(&pNewDest[i * lockedDestination->Stride()], &pOldDest[i * lockedSource->Stride()], width * PixelSize());
		}
	}

	void Surface::copyConvertAnyTo32( Surface::Ptr source, RectInt &sourceAreaToCopy, PointInt destinationTopLeft ) {
		auto clippedSourceArea = Geom::ClipSource<int>(
			{ { 0, 0 }, GetSize() },
			destinationTopLeft,
			{ { 0, 0 }, source->GetSize() },
			sourceAreaToCopy.TopLeft(),
			sourceAreaToCopy.Dimensions());

		LockedArea::Ptr lockedDestination = LockSurface({ destinationTopLeft, clippedSourceArea.Dimensions() });
		uint8_t* pNewDest = lockedDestination->Buffer();

		source->CopyRectToBuffer(pNewDest, lockedDestination->Stride(), sourceAreaToCopy, Img::Color(0xff, 0xff, 0xff, 0xff), 0, 10, 10, false);
	}

	void Surface::onCopyAreaToBuffer(uint8_t* pDest, int strideDest, const RectInt& sourceRect, const Img::Color& bgrColor, int brightness, int contrast, int gamma, bool applyAlpha) {
		const SizeInt& dstSize(sourceRect.Dimensions());

		// Set up destination buffer
		LockedArea::Ptr lockedSource = LockSurface();

		Filter::FilterBuffer src(
			GetSize(),
			PixelSize(),
			lockedSource->Buffer(),
			lockedSource->Stride(),
			GetPalette());
		Filter::FilterBuffer dst(
			SizeInt(strideDest / 4, dstSize.Height),
			4,
			pDest,
			strideDest);
		Filter::Scale::AutoFilter(Filter::Mode::DirectCopy, src, dst, sourceRect, GetFormat(), 1.0f);

		if (gamma != 10) {
			Filter::Adjust::Gamma(dst, dst, RectInt(PointInt(0, 0), dst.Dimensions), PointInt(0, 0), gamma);
		}

		if ((brightness != 0) || (contrast != Img::ContrastStep)) {
			Filter::Adjust::BrightnessContrast(dst, dst, RectInt(PointInt(0, 0), dst.Dimensions), PointInt(0, 0), brightness, contrast);
		}

		if (HasAlpha(GetFormat()) && applyAlpha) {
			Filter::Alpha::SolidColor(dst, dst, RectInt(PointInt(0, 0), dst.Dimensions), PointInt(0, 0), bgrColor);
		}
	}

	void Surface::onBlitSurfaceColorKey(Surface::Ptr source, RectInt sourceAreaToCopy, PointInt destinationTopLeft, uint8_t colorKeyIndex) {
		LockedArea::Ptr lockedSource = source->LockSurface();
		LockedArea::Ptr lockedDestination = LockSurface();

		Filter::FilterBuffer src(
			source->GetSize(),
			source->PixelSize(),
			lockedSource->Buffer(),
			lockedSource->Stride(),
			source->GetPalette());
		Filter::FilterBuffer dst(
			GetSize(),
			PixelSize(),
			lockedDestination->Buffer(),
			lockedDestination->Stride());
		Filter::Alpha::IndexedColorKey(src, dst, sourceAreaToCopy, destinationTopLeft, source->GetFormat(), colorKeyIndex);
	}

	void Surface::FillRect(const RectInt& areaToFill, const Img::Color& colorToSet) {
		onFillRect(areaToFill, colorToSet);
	}

	void Surface::onFillRect(const RectInt& areaToFill, const Color& colorToSet) {
		auto lockedDestination = LockSurface(areaToFill);
		if (lockedDestination == nullptr)
		{
			DO_THROW(Err::CriticalError, "Surface is not yet allocated");
		}
		uint8_t* pNewDest	= lockedDestination->Buffer();

		size_t scans	= areaToFill.Height();
		size_t width	= areaToFill.Width();
		size_t stride	= lockedDestination->Stride();
		
		if(PixelSize() == 1) {
			size_t rowsize	= width * PixelSize();
			for(size_t y = 0; y < scans; ++y) {
				memset(pNewDest, colorToSet.A, rowsize);
				pNewDest += stride;
			}
		}
		else if (GetFormat() == Format::RGB565) {
			uint16_t toSet = ToRGBWord565(colorToSet);

			for(size_t y = 0; y < scans; ++y) {
				uint16_t* currentPixel = reinterpret_cast<uint16_t*>(pNewDest);

				for(size_t x = 0; x < width; ++x) *(currentPixel++) = toSet;
				pNewDest += stride;
			}
		}
		else if ((GetFormat() == Format::XRGB1555) || (GetFormat() == Format::ARGB1555)) {
			uint16_t	toSet = ToARGBWord1555(colorToSet);

			for(size_t y = 0; y < scans; ++y) {
				uint16_t* currentPixel = reinterpret_cast<uint16_t*>(pNewDest);
				for(size_t x = 0; x < width; ++x) *(currentPixel++) = toSet;
				pNewDest += stride;
			}
		}
		else if (PixelSize() == 4) {
			uint32_t	toSet = ToARGBDWORD(colorToSet);

			for(size_t y = 0; y < scans; ++y) {
				uint32_t* currentPixel = reinterpret_cast<uint32_t*>(pNewDest);

				for(size_t x = 0; x < width; ++x) *(currentPixel++) = toSet;
				pNewDest += stride;
			}
		}
	}

	void Surface::Deallocate() {
		onDeallocate();
		m_isCreated = false;
		m_size = Geom::SizeInt(0, 0);
		m_swFormat = Format::Undefined;
		m_pixelSize = 0;
	}

	size_t Surface::LockedArea::Stride() const {
		return m_stride;
	}

	FilterBufferAndLock GenerateFilterBuffer(Surface::Ptr src) {
		FilterBufferAndLock fbl;
		fbl.lock = src->LockSurface(Img::LockRead);
		if (fbl.lock == nullptr) {
			return fbl;
		}
		fbl.filterBuffer.Construct(src->GetSize(), src->PixelSize(), fbl.lock->Buffer(), fbl.lock->Stride(), src->GetPalette());
		return fbl;
	}

	Geom::SizeInt CalculateUnzoomedSize(std::shared_ptr<Surface> surf, Filter::RotationAngle angle) {
		if (surf == nullptr) {
			return Geom::SizeInt{ 0, 0 };
		}

		return Filter::CalculateUnzoomedSize(surf->GetSize(), angle);
	}
}
