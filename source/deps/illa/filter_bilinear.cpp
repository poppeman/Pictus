#include "filter_bilinear.h"
#include "filter_int.h"

#define BSHIFT		11
#define BSHIFTx2	22
#define BMUL		2048

namespace Filter {
	namespace Scale {
		using namespace Geom;

		namespace Internal {
			void set_contrib(std::vector<Contrib>& contrib, uint32_t i, uint32_t max_i, uint32_t max_coord, uint32_t ofs) {
				uint32_t floor = ofs >> BSHIFT;

				if (i < max_i) {
					contrib[i].floor = floor;
					if ((contrib[i].floor + 1) >= max_coord) {
						contrib[i].floor = std::max<uint32_t>(2, max_coord) - 2;
						ofs = (contrib[i].floor << BSHIFT) + BMUL - 2;
					}
				}

				contrib[i].frac = ofs - (contrib[i].floor << BSHIFT);
				contrib[i].frac_inv = BMUL - contrib[i].frac;
			}
		}

		struct PaletteToDWord {
			PaletteToDWord(const Img::Palette& p) :m_p(p) {}

			uint32_t Assemble(uint8_t ul, uint32_t fUL, uint8_t ur, uint32_t fUR, uint8_t bl, uint32_t fBL, uint8_t br, uint32_t fBR) const {
				const Img::Color& cUL = m_p.Color(ul);
				const Img::Color& cUR = m_p.Color(ur);
				const Img::Color& cBL = m_p.Color(bl);
				const Img::Color& cBR = m_p.Color(br);

				uint32_t A = (cUL.A * fUL + cUR.A * fUR + cBL.A * fBL + cBR.A * fBR) >> BSHIFTx2;
				uint32_t R = (cUL.R * fUL + cUR.R * fUR + cBL.R * fBL + cBR.R * fBR) >> BSHIFTx2;
				uint32_t G = (cUL.G * fUL + cUR.G * fUR + cBL.G * fBL + cBR.G * fBR) >> BSHIFTx2;
				uint32_t B = (cUL.B * fUL + cUR.B * fUR + cBL.B * fBL + cBR.B * fBR) >> BSHIFTx2;

				return Img::ToARGBDWORD(A, R, G, B);
			}

			const Img::Palette& m_p;

			void operator=(const PaletteToDWord&) = delete;
		};

		template <typename T, class U> void performBilinear(const FilterBuffer& source, FilterBuffer& dest, const Geom::RectInt& region, float zoom, U pixelConverter) {
			int width = region.Width();
			int height = region.Height();
			using namespace Internal;


			// Precalculate stuff
			std::vector<Contrib> x_contrib(width);
			std::vector<Contrib> y_contrib(height);

			for(int i = 0; i < width; ++i) {
                set_contrib(x_contrib, i, width, source.Dimensions.Width, static_cast<uint32_t>(((region.Left() + i + 0.25f) / zoom) * BMUL));
            }

			for(int i = 0; i < height; ++i) {
                set_contrib(y_contrib, i, height, source.Dimensions.Height, static_cast<uint32_t>(((region.Top() + i + 0.25f) / zoom) * BMUL));
            }

			uint8_t* destCurrentScanlinePtr = dest.BufferData;
			int rw = region.Width();

			for (int y = 0; y < region.Height(); ++y) {
				uint32_t floorY = y_contrib[y].floor;
				uint32_t iFracY = y_contrib[y].frac;
				uint32_t iFracYinv	= y_contrib[y].frac_inv;

				const T* currSourceScanUpper = reinterpret_cast<T*>(source.BufferData + floorY * source.Stride);
				const T* currSourceScanLower = reinterpret_cast<T*>(source.BufferData + (floorY + 1) * source.Stride);

				uint32_t* currDestPixel = reinterpret_cast<uint32_t*>(destCurrentScanlinePtr);

				for(int x = 0; x < rw; ++x) {
					uint32_t iFracX	= x_contrib[x].frac;
					uint32_t iFracXinv	= x_contrib[x].frac_inv;
					uint32_t fUL		= (iFracXinv * iFracYinv);
					uint32_t fUR		= (iFracX * iFracYinv);
					uint32_t fBL		= (iFracXinv * iFracY);
					uint32_t fBR		= (iFracX * iFracY);

					*(currDestPixel++) = pixelConverter.Assemble(
						currSourceScanUpper[x_contrib[x].floor], fUL,
						currSourceScanUpper[x_contrib[x].floor + 1], fUR,
						currSourceScanLower[x_contrib[x].floor], fBL,
						currSourceScanLower[x_contrib[x].floor + 1], fBR);
				}
				destCurrentScanlinePtr += dest.Stride;
			}
		}

		void Bilinear(const FilterBuffer& source, FilterBuffer& dest, const Geom::RectInt& region, Img::Format format, float zoom) {
			if (format == Img::Format::XRGB8888) {
				struct DWordToDWord {
					uint32_t Assemble(uint32_t ul, uint32_t fUL, uint32_t ur, uint32_t fUR, uint32_t bl, uint32_t fBL, uint32_t br, uint32_t fBR) const {
						return Img::ToARGBDWORD(
							0xff,
							(Img::ChannelARGB8888Red(ul) * fUL + Img::ChannelARGB8888Red(ur) * fUR + Img::ChannelARGB8888Red(bl) * fBL + Img::ChannelARGB8888Red(br) * fBR) >> BSHIFTx2,
							(Img::ChannelARGB8888Green(ul) * fUL + Img::ChannelARGB8888Green(ur) * fUR + Img::ChannelARGB8888Green(bl) * fBL + Img::ChannelARGB8888Green(br) * fBR) >> BSHIFTx2,
							(Img::ChannelARGB8888Blue(ul) * fUL + Img::ChannelARGB8888Blue(ur) * fUR + Img::ChannelARGB8888Blue(bl) * fBL + Img::ChannelARGB8888Blue(br) * fBR) >> BSHIFTx2);
					}
				};
				performBilinear<uint32_t>(source, dest, region, zoom, DWordToDWord());
			}
			else if (format == Img::Format::ARGB8888) {
				struct DWordToDWord {
					uint32_t Assemble(uint32_t ul, uint32_t fUL, uint32_t ur, uint32_t fUR, uint32_t bl, uint32_t fBL, uint32_t br, uint32_t fBR) const {
						return Img::ToARGBDWORD(
							(Img::ChannelARGB8888Alpha(ul) * fUL + Img::ChannelARGB8888Alpha(ur) * fUR + Img::ChannelARGB8888Alpha(bl) * fBL + Img::ChannelARGB8888Alpha(br) * fBR) >> BSHIFTx2,
							(Img::ChannelARGB8888Red(ul) * fUL + Img::ChannelARGB8888Red(ur) * fUR + Img::ChannelARGB8888Red(bl) * fBL + Img::ChannelARGB8888Red(br) * fBR) >> BSHIFTx2,
							(Img::ChannelARGB8888Green(ul) * fUL + Img::ChannelARGB8888Green(ur) * fUR + Img::ChannelARGB8888Green(bl) * fBL + Img::ChannelARGB8888Green(br) * fBR) >> BSHIFTx2,
							(Img::ChannelARGB8888Blue(ul) * fUL + Img::ChannelARGB8888Blue(ur) * fUR + Img::ChannelARGB8888Blue(bl) * fBL + Img::ChannelARGB8888Blue(br) * fBR) >> BSHIFTx2);
					}
				};
				performBilinear<uint32_t>(source, dest, region, zoom, DWordToDWord());
			}
			else if (format == Img::Format::XRGB1555) {
				struct WordToDWord {
					uint32_t Assemble(uint16_t ul, uint32_t fUL, uint16_t ur, uint32_t fUR, uint16_t bl, uint32_t fBL, uint16_t br, uint32_t fBR) const {
						return Img::ToARGBDWORD(
							0xff,
							((Img::ChannelARGB1555Red(ul) * fUL + Img::ChannelARGB1555Red(ur) * fUR + Img::ChannelARGB1555Red(bl) * fBL + Img::ChannelARGB1555Red(br) * fBR) >> BSHIFTx2) << 3,
							((Img::ChannelARGB1555Green(ul) * fUL + Img::ChannelARGB1555Green(ur) * fUR + Img::ChannelARGB1555Green(bl) * fBL + Img::ChannelARGB1555Green(br) * fBR) >> BSHIFTx2) << 3,
							((Img::ChannelARGB1555Blue(ul) * fUL + Img::ChannelARGB1555Blue(ur) * fUR + Img::ChannelARGB1555Blue(bl) * fBL + Img::ChannelARGB1555Blue(br) * fBR) >> BSHIFTx2) << 3);
					}
				};
				performBilinear<uint16_t>(source, dest, region, zoom, WordToDWord());
			}
			else if (format == Img::Format::ARGB1555) {
				struct WordToDWord {
					uint32_t Assemble(uint16_t ul, uint32_t fUL, uint16_t ur, uint32_t fUR, uint16_t bl, uint32_t fBL, uint16_t br, uint32_t fBR) const {
						return Img::ToARGBDWORD(
							255 * ((Img::ChannelARGB1555Alpha(ul) * fUL + Img::ChannelARGB1555Alpha(ur) * fUR + Img::ChannelARGB1555Alpha(bl) * fBL + Img::ChannelARGB1555Alpha(br) * fBR) >> BSHIFTx2),
							((Img::ChannelARGB1555Red(ul) * fUL + Img::ChannelARGB1555Red(ur) * fUR + Img::ChannelARGB1555Red(bl) * fBL + Img::ChannelARGB1555Red(br) * fBR) >> BSHIFTx2) << 3,
							((Img::ChannelARGB1555Green(ul) * fUL + Img::ChannelARGB1555Green(ur) * fUR + Img::ChannelARGB1555Green(bl) * fBL + Img::ChannelARGB1555Green(br) * fBR) >> BSHIFTx2) << 3,
							((Img::ChannelARGB1555Blue(ul) * fUL + Img::ChannelARGB1555Blue(ur) * fUR + Img::ChannelARGB1555Blue(bl) * fBL + Img::ChannelARGB1555Blue(br) * fBR) >> BSHIFTx2) << 3);
					}
				};
				performBilinear<uint16_t>(source, dest, region, zoom, WordToDWord());
			}
			else if (format == Img::Format::RGB565) {
				struct WordToDWord {
					uint32_t Assemble(uint16_t ul, uint32_t fUL, uint16_t ur, uint32_t fUR, uint16_t bl, uint32_t fBL, uint16_t br, uint32_t fBR) const {
						return Img::ToARGBDWORD(
							0xff,
							((Img::ChannelRGB565Red(ul) * fUL + Img::ChannelRGB565Red(ur) * fUR + Img::ChannelRGB565Red(bl) * fBL + Img::ChannelRGB565Red(br) * fBR) >> BSHIFTx2) << 3,
							((Img::ChannelRGB565Green(ul) * fUL + Img::ChannelRGB565Green(ur) * fUR + Img::ChannelRGB565Green(bl) * fBL + Img::ChannelRGB565Green(br) * fBR) >> BSHIFTx2) << 2,
							((Img::ChannelRGB565Blue(ul) * fUL + Img::ChannelRGB565Blue(ur) * fUR + Img::ChannelRGB565Blue(bl) * fBL + Img::ChannelRGB565Blue(br) * fBR) >> BSHIFTx2)) << 3;
					}
				};
				performBilinear<uint16_t>(source, dest, region, zoom, WordToDWord());
			}
			else if (format == Img::Format::Index8) {
				performBilinear<uint8_t>(source, dest, region, zoom, PaletteToDWord(source.Palette));
			}
			else
				DO_THROW(Err::InvalidParam, L"Format not supported:" + ToWString(format));
		}
	}
}
