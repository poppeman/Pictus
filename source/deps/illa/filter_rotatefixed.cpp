#include "StdAfx.h"

#include "filter_int.h"

namespace Filter {
	namespace Transformation {
		_Check_return_ bool IsInplace( _In_ RotationAngle angle ) {
			switch(angle) {
				case RotateDefault:
				case FlipX:
				case FlipY:
				case Rotate180:
					return true;
			}
			return false;
		}

		void QuerySizeRotateFixed(const FilterBuffer& source, RotationAngle angle, Geom::SizeInt* requiredSize, bool* supportInPlace) {
			bool doSupport = true;
			Geom::SizeInt reqSize = source.Dimensions;

			switch(angle) {
				case RotateDefault:
				case FlipX:
				case FlipY:
				case Rotate180:
					break;

				default:
					doSupport = false;
					reqSize = source.Dimensions.Flipped();
					break;
			}

			if (supportInPlace) {
				*supportInPlace = doSupport;
			}

			if (requiredSize) {
				*requiredSize = reqSize;
			}
		}

		void mirror(const FilterBuffer& source, FilterBuffer& dest, const Geom::RectInt& region, const Geom::PointInt& destTopLeft) {
			if ((source == dest) && (region.TopLeft() != destTopLeft)) {
				DO_THROW(Err::InvalidParam, TX("Inplace mirroring requires matching regions."));
			}

			size_t srcStride = source.Stride;
			size_t dstStride = dest.Stride;

			if (source == dest) {
				uint8_t* bArray = source.BufferData + (4 * region.Left() + region.Top() * srcStride);

				for (int y = 0; y < region.Height(); ++y) {
					uint32_t* dwArray1 = reinterpret_cast<uint32_t*>(bArray);
					uint32_t* dwArray2 = reinterpret_cast<uint32_t*>(bArray + 4 * (region.Width() - 1));

					for (int x = 0; x < (region.Width() / 2); ++x) {
						std::swap(*(dwArray1++), *(dwArray2--));
					}

					bArray += srcStride;
				}
			}
			else {
				uint8_t* bSrcArray = source.BufferData + (4 * region.Left() + region.Top() * srcStride);
				uint8_t* bDstArray = dest.BufferData + (4 * destTopLeft.X + destTopLeft.Y * dstStride);

				for (int y = 0; y < region.Height(); ++y) {
					uint32_t* dwArray1 = reinterpret_cast<uint32_t*>(bSrcArray);
					uint32_t* dwArray2 = reinterpret_cast<uint32_t*>(bDstArray + 4 * (region.Width() - 1));

					for (int x = 0; x < region.Width(); ++x) {
						*(dwArray2--) = *(dwArray1++);
					}

					bSrcArray += srcStride;
					bDstArray += dstStride;
				}
			}
		}

		void flip(const FilterBuffer& source, FilterBuffer& dest, const Geom::RectInt& region, const Geom::PointInt& destTopLeft) {
			if ((source == dest) && (region.TopLeft() != destTopLeft)) {
				DO_THROW(Err::InvalidParam, TX("Inplace flipping requires matching regions."));
			}

			size_t srcStride = source.Stride;
			size_t dstStride = dest.Stride;

			int toCopy = region.Width() * 4;

			if (source.BufferData == dest.BufferData) {
				std::vector<uint8_t> pRow(toCopy);

				int height = region.Height();

				uint8_t* bSrcArray = source.BufferData + (4 * region.Left() + region.Top() * srcStride);
				uint8_t* bDstArray = bSrcArray + (region.Height() - 1) * srcStride;

				for (int i = 0; i < (height / 2); ++i) {
					memcpy(&pRow[0], bSrcArray, toCopy);
					memcpy(bSrcArray, bDstArray, toCopy);
					memcpy(bDstArray, &pRow[0], toCopy);

					bSrcArray += srcStride;
					bDstArray -= srcStride;
				}
			}
			else {
				uint8_t* bSrcArray = source.BufferData + (4 * region.Left() + region.Top() * srcStride);
				uint8_t* bDstArray = dest.BufferData + (4 * destTopLeft.X + destTopLeft.Y * dstStride) + (region.Height() - 1) * dstStride;

				for (int y = 0; y < region.Height(); ++y) {
					memcpy(bDstArray, bSrcArray, toCopy);

					bSrcArray += srcStride;
					bDstArray -= dstStride;
				}
			}
		}

		void rot90(const FilterBuffer& source, FilterBuffer& dest, const Geom::RectInt& region, const Geom::PointInt& destTopLeft) {
			if (source.BufferData == dest.BufferData) {
				DO_THROW(Err::InvalidParam, TX("Inplace rotation not supported."));
			}

			uint8_t* bSrcArray = source.BufferData + (4 * region.Left() + region.Top() * source.Stride);
			uint8_t* bDstArray = dest.BufferData + (4 * destTopLeft.X + destTopLeft.Y * dest.Stride);

			for (int y = 0; y < region.Height(); ++y) {
				uint32_t* dwArray1 = reinterpret_cast<uint32_t*>(bSrcArray);

				for (int x = 0; x < region.Width(); ++x) {
					*reinterpret_cast<uint32_t*>(bDstArray + 4 * (region.Height() - y - 1) + x * dest.Stride) = *(dwArray1++);
				}
				bSrcArray += source.Stride;
			}
		}

		void rot270(const FilterBuffer& source, FilterBuffer& dest, const Geom::RectInt& region, const Geom::PointInt& destTopLeft) {
			if (source.BufferData == dest.BufferData) {
				DO_THROW(Err::InvalidParam, TX("Inplace rotation not supported."));
			}

			uint8_t* bSrcArray = source.BufferData + (4 * region.Left() + region.Top() * source.Stride);
			uint8_t* bDstArray = dest.BufferData + (4 * destTopLeft.X + destTopLeft.Y * dest.Stride);

			for (int y = 0; y < region.Height(); ++y) {
				uint32_t* dwArray1 = reinterpret_cast<uint32_t*>(bSrcArray);

				for (int x = 0; x < region.Width(); ++x) {
					*reinterpret_cast<uint32_t*>(bDstArray + 4 * y + (region.Width() - x - 1) * dest.Stride) = *(dwArray1++);
				}
				bSrcArray += source.Stride;
			}
		}

		void RotateFixed(const FilterBuffer& source, FilterBuffer& dest, const Geom::RectInt& region, const Geom::PointInt& destTopLeft, RotationAngle angle) {
			if (source.BufferData == nullptr || dest.BufferData == nullptr) {
				DO_THROW(Err::InvalidParam, TX("Null buffer not allowed."));
			}

			switch(angle) {
				case Filter::RotateDefault:
					if (source.BufferData != dest.BufferData) {
						DO_THROW(Err::InvalidParam, TX("Operation not yet supported (inplace non-rotation)"));
					}
					break;
				case Filter::FlipX:
					mirror(source, dest, region, destTopLeft);
					break;
				case Filter::FlipY:
					flip(source, dest, region, destTopLeft);
					break;
				case Filter::Rotate90:
					rot90(source, dest, region, destTopLeft);
					break;
				case Filter::Rotate180:
					mirror(source, dest, region, destTopLeft);
					flip(dest, dest, Geom::RectInt(destTopLeft, region.Dimensions()), destTopLeft);
					break;

				case Filter::Rotate270:
					rot270(source, dest, region, destTopLeft);
					break;

				default:
					DO_THROW(Err::InvalidParam, TX("Rotation angle not supported:") + ToWString(angle));
			}
		}
	}
}
