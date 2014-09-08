#include "StdAfx.h"
#include "filter_int.h"

namespace Filter {
	namespace Scale {
		namespace Internal {
			struct ARGBFloat { float A, R, G, B; };

			ARGBFloat operator*(const ARGBFloat& lhs, float rhs) { ARGBFloat r={lhs.A*rhs, lhs.R*rhs, lhs.G*rhs, lhs.B*rhs}; return r; }
			ARGBFloat& operator+=(ARGBFloat& lhs, const ARGBFloat& rhs) {
				lhs.A += rhs.A;
				lhs.R += rhs.R;
				lhs.G += rhs.G;
				lhs.B += rhs.B;
				return lhs;
			}
#pragma warning(push)
#pragma warning(disable:4244)
			inline void NormalizeARGB(ARGBFloat& f, float mul) {
				f.A = Util::Constrain(0.0f, f.A*mul, 255.0f);
				f.G = Util::Constrain(0.0f, f.G*mul, 255.0f);
				f.B = Util::Constrain(0.0f, f.B*mul, 255.0f);
				f.R = Util::Constrain(0.0f, f.R*mul, 255.0f);
			}

			inline void NormalizeXRGB(ARGBFloat& f, float mul) {
				f.R = Util::Constrain(0.0f, f.R*mul, 255.0f);
				f.G = Util::Constrain(0.0f, f.G*mul, 255.0f);
				f.B = Util::Constrain(0.0f, f.B*mul, 255.0f);
			}

			struct ConvertARGB8888 {
				ARGBFloat operator()(const uint32_t& v) const {
					Img::Color c = Img::Color::FromDWord(v);
					ARGBFloat f={c.A, c.R, c.G, c.B};
					return f;
				}
				void Normalize(ARGBFloat& f, float mul) {
					NormalizeARGB(f, mul);
				}
			};

			struct ConvertXRGB8888 {
				ARGBFloat operator()(const uint32_t& v) const {
					Img::Color c = Img::Color::FromDWord(v);
					ARGBFloat f={255.0f, c.R, c.G, c.B};
					return f;
				}
				void Normalize(ARGBFloat& f, float mul) {
					NormalizeXRGB(f, mul);
				}
			};

			struct ConvertARGB1555 {
				ARGBFloat operator()(const uint16_t& v) const {
					Img::Color c = Img::Color::FromDWord(Img::ConvertARGB1555ToARGB8888(v));
					ARGBFloat f={c.A, c.R, c.G, c.B};
					return f;
				}
				void Normalize(ARGBFloat& f, float mul) {
					NormalizeARGB(f, mul);
				}
			};

			struct ConvertXRGB1555 {
				ARGBFloat operator()(const uint16_t& v) const {
					Img::Color c = Img::Color::FromDWord(Img::ConvertXRGB1555ToARGB8888(v));
					ARGBFloat f={255.0f, c.R, c.G, c.B};
					return f;
				}
				void Normalize(ARGBFloat& f, float mul) {
					NormalizeXRGB(f, mul);
				}
			};

			struct ConvertRGB565 {
				ARGBFloat operator()(const uint16_t& v) const {
					Img::Color c = Img::Color::FromDWord(Img::ConvertRGB565ToARGB8888(v));
					ARGBFloat f={255.0f, c.R, c.G, c.B};
					return f;
				}
				void Normalize(ARGBFloat& f, float mul) {
					NormalizeXRGB(f, mul);
				}
			};

			struct ConvertIndexed {
				ConvertIndexed(const Img::Palette& p):m_p(p) {}
				ARGBFloat operator()(const uint8_t& v) const {
					const Img::Color& c = m_p.Color(v);
					ARGBFloat f={c.A, c.R, c.G, c.B};
					return f;
				}
				void Normalize(ARGBFloat& f, float mul) {
					NormalizeARGB(f, mul);
				}
				void operator=(const ConvertIndexed&) = delete;
				const Img::Palette& m_p;
			};
#pragma warning(pop)
			using namespace Geom;

			struct CSE {
				float center;
				int start;
				int end;
				int num;
			};

			CSE determineCSE(int i, int max, float support, float zoom) {
				CSE c;
				c.center = (i + 0.5f) / zoom;
				c.start = Util::Max<int>(0, static_cast<int>(c.center - support + 0.5f));
				c.end = Util::Min(max, static_cast<int>(c.center + support + 0.5f));
				if(c.start > c.end) c.start = c.end;
				c.num = c.end - c.start;
				return c;
			}

			template <typename T, int contribOrigo, class U>
			inline uint32_t processPixel(const FilterBuffer& source, const CSE& cseX, const CSE& cseY, const std::vector<float>& xcontribs, int& xindex, const std::vector<float>& ycontribs, const int yindex, U& pixelConverter) {
				ARGBFloat cDest = {0};

				float density = 0.0f;

				uint8_t* currSourceScanByte = source.BufferData + cseY.start * source.Stride;
				for(int ny = 0; ny < cseY.num; ++ny) {
					float ycontrib = ycontribs[yindex + ny];
					T* currSourceScan = reinterpret_cast<T*>(currSourceScanByte) + cseX.start;
					for(int nx = 0; nx < cseX.num; ++nx) {
						float contrib = xcontribs[xindex + nx] * ycontrib;
						density += contrib;
						cDest += pixelConverter(*(currSourceScan++)) * contrib;
					}
					currSourceScanByte += source.Stride;
				}
				if (density != 0.0f && density != 1.0f)
					pixelConverter.Normalize(cDest, 1.0f/density);

				xindex += cseX.num;

				return Img::ToARGBDWORD((int)cDest.A, (int)cDest.R, (int)cDest.G, (int)cDest.B);
			}

			template <typename T, int KernelSize, int contribOrigo, class U>
			void processScanline(const FilterBuffer& source, const PointInt& dstTl, uint32_t* dest, int y, int rw, float zoom, U& pixelConverter, const std::vector<float>& xcontribs, const std::vector<float>& ycontribs, int& yindex) {
				CSE cseY = determineCSE(dstTl.Y + y, source.Dimensions.Height, KernelSize, zoom);
				int xindex = 0;

				for (int x = 0; x < rw; ++x) {
					*(dest++) = processPixel<T, contribOrigo>(
						source, 
						determineCSE(dstTl.X + x, source.Dimensions.Width, KernelSize, zoom), cseY, 
						xcontribs,
						xindex,
						ycontribs, 
						yindex,
						pixelConverter);
				}
				yindex += cseY.num;
			}

			template <int _NS>
			void precalcContributionAxis(std::vector<float>& dest, int tl, int regdim, int srcdim, const float* kernelFunction, float KernelSize, float zoom, float scaleFactor) {
				int i = 0;
				for(int x=0; x<regdim; ++x) {
					CSE cseX = determineCSE(tl + x, srcdim, KernelSize, zoom);
					float sx = 0.5f + cseX.start - cseX.center;
					for(int nx = 0; nx < cseX.num; ++nx, ++sx)
						dest[i++] = kernelFunction[_NS / 2 + (int)(sx * scaleFactor)];
				}
			}

			template <int _NS>
			void precalcContributions(std::vector<float>& destX, std::vector<float>& destY, const Geom::RectInt& region, const Geom::SizeInt& dims, const float* kernelFunction, float KernelSize, float zoom, float scaleFactor) {
				precalcContributionAxis<_NS>(destX, region.TopLeft().X, region.Dimensions().Width, dims.Width, kernelFunction, KernelSize, zoom, scaleFactor);
				precalcContributionAxis<_NS>(destY, region.TopLeft().Y, region.Dimensions().Height, dims.Height, kernelFunction, KernelSize, zoom, scaleFactor);
			}

			template <typename T, int _NS, int KernelSize, class U>
			inline void performFilter(const FilterBuffer& source, FilterBuffer& dest, const Geom::RectInt& region, float zoom, U pixelConverter, const float* kernelFunction) {
				uint8_t* destCurrentScanlinePtr = dest.BufferData;
				int rh = region.Height();
				int rw = region.Width();

				const float scaleFactor = ((_NS * 0.5f) / (float)KernelSize) * Util::Min(1.0f, zoom);

				// +1, might become one extra sample when hitting the exact center of a pixel.
				Geom::SizeInt numContribs = Geom::SizeInt(rw, rh) * (KernelSize + 1) * 2;
				std::vector<float> xcontribs(numContribs.Width);
				std::vector<float> ycontribs(numContribs.Height);

				precalcContributions<_NS>(xcontribs, ycontribs, region, source.Dimensions, kernelFunction, KernelSize, zoom, scaleFactor);

				int yindex = 0;
				for (int y = 0; y < rh; ++y) {
					uint32_t* currDestPixel = reinterpret_cast<uint32_t*>(destCurrentScanlinePtr);
					processScanline<T, KernelSize, (int)(0.5 * _NS)>(source, region.TopLeft(), currDestPixel, y, rw, zoom, pixelConverter, xcontribs, ycontribs, yindex);
					destCurrentScanlinePtr += dest.Stride;
				}
			}

			template <int _NS, int KernelSize>
			void GenericFilter(const FilterBuffer& source, FilterBuffer& dest, const Geom::RectInt& region, Img::Format format, float zoom, const float* kernelFunction) {
				switch(format) {
					case Img::Format::ARGB8888:
						performFilter<uint32_t, _NS, KernelSize>(source, dest, region, zoom, ConvertARGB8888(), kernelFunction);
						break;
					case Img::Format::XRGB8888:
						performFilter<uint32_t, _NS, KernelSize>(source, dest, region, zoom, ConvertXRGB8888(), kernelFunction);
						break;
					case Img::Format::ARGB1555:
						performFilter<uint16_t, _NS, KernelSize>(source, dest, region, zoom, ConvertARGB1555(), kernelFunction);
						break;
					case Img::Format::XRGB1555:
						performFilter<uint16_t, _NS, KernelSize>(source, dest, region, zoom, ConvertXRGB1555(), kernelFunction);
						break;
					case Img::Format::RGB565:
						performFilter<uint16_t, _NS, KernelSize>(source, dest, region, zoom, ConvertRGB565(), kernelFunction);
						break;
					case Img::Format::Index8:
						performFilter<uint8_t, _NS, KernelSize>(source, dest, region, zoom, ConvertIndexed(source.Palette), kernelFunction);
						break;
					default:
						DO_THROW(Err::InvalidParam, TX("Format not supported:") + ToWString(format));
				}
			}
		}
	}
}
