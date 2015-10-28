#include "c_png.h"
#include "surface.h"
#include "surface_locked_area.h"
#include "surfacemgr.h"
#include "orz/logger.h"

namespace Img {
	using namespace Geom;

	void CodecPNG::libpng_error(png_structp png_ptr, png_const_charp message) {
		// setjmp/getjmp should be put where the sun doesn't shine.
		DO_THROW(Err::CodecError, "Unknown error.");
	}

	bool CodecPNG::PerformLoadHeader(IO::FileReader::Ptr file, ImageInfo& info) {
		try {
			uint8_t sign[SignBytes];
			if (file->Read(sign, 1, SignBytes) != SignBytes) {
				Log << "(CodecPNG::PerformLoadHeader) Signature bytes missing\n";
				return false;
			}

			if (png_sig_cmp(sign, 0, SignBytes) != 0) {
				Log << "(CodecPNG::PerformLoadHeader) Sign bytes mismatch\n";
				return false;
			}

			// Create png structs
			m_png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
			if (m_png_ptr == 0) {
				return false;
			}

			m_info_ptr = png_create_info_struct(m_png_ptr);
			if (m_info_ptr == 0) {
				return false;
			}

			png_set_error_fn(m_png_ptr, 0, libpng_error, 0);

			// Yay, void only :/
//			png_init_io(m_png_ptr, (FILE*)file->GetInternalPtr());
			png_set_read_fn(m_png_ptr, reinterpret_cast<void*>(file.get()), libpng_user_read_data);
			png_set_sig_bytes(m_png_ptr, 8);
			png_read_info(m_png_ptr, m_info_ptr);

			png_uint_32 w, h;
			int interlace_type, compression_type, filter_type, type;
			png_get_IHDR(m_png_ptr, m_info_ptr, &w, &h, &m_bitdepth, &type, &interlace_type, &compression_type, &filter_type);
			m_chans=png_get_channels(m_png_ptr, m_info_ptr);

			info.Dimensions = SizeInt(w, h);

			// Add alpha for RGB images
			if (type == PNG_COLOR_TYPE_RGB) png_set_add_alpha(m_png_ptr, 255, PNG_FILLER_AFTER);
			png_set_bgr(m_png_ptr);

			if (m_bitdepth == 16) {
				png_set_strip_16(m_png_ptr);	// No support for 16-bit channels
				m_bitdepth = 8;
			}

			if (m_bitdepth < 8) {
				if (type == PNG_COLOR_TYPE_GRAY) {
					png_set_packing(m_png_ptr);
					png_set_expand(m_png_ptr);
					if (png_get_valid(m_png_ptr, m_info_ptr, PNG_INFO_tRNS)) {
						png_set_tRNS_to_alpha(m_png_ptr);
						png_set_gray_to_rgb(m_png_ptr);
						png_read_update_info(m_png_ptr, m_info_ptr);
						png_get_IHDR(m_png_ptr, m_info_ptr, &w, &h, &m_bitdepth, &type, &interlace_type, &compression_type, &filter_type);
						m_chans	= png_get_channels(m_png_ptr, m_info_ptr);
					}
				}
				else if (type == PNG_COLOR_TYPE_PALETTE) {
					png_set_packing(m_png_ptr);
				}
			}

			// TODO: Add support for PNG_COLOR_TYPE_ALPHA as well
			if ((type == PNG_COLOR_TYPE_RGB) && (m_bitdepth == 8)) {
				info.SurfaceFormat = Img::Format::XRGB8888;
			}
			else if ((type == PNG_COLOR_TYPE_GRAY) && (m_bitdepth <= 8)) {
				info.SurfaceFormat = Img::Format::Index8;
				m_palette = Grayscale();
			}
			else if ((type == PNG_COLOR_TYPE_RGB_ALPHA) && (m_bitdepth <= 8)) {
				info.SurfaceFormat = Img::Format::ARGB8888;
			}
			else if ((type == PNG_COLOR_TYPE_PALETTE) && (m_bitdepth <= 8)) {
				info.SurfaceFormat = Img::Format::Index8;
				int numCol;
				png_color* pal;
				if (png_get_PLTE(m_png_ptr, m_info_ptr, &pal, &numCol)) {
					for(int i = 0; i < numCol; ++i)
						m_palette.Color(i, Color(255, pal[i].red, pal[i].green, pal[i].blue));

					png_byte* alpha;
					if (png_get_tRNS(m_png_ptr, m_info_ptr, &alpha, &numCol, 0))
						for(int i = 0; i < numCol; ++i) {
							Color c = m_palette.Color(i);
							c.A = alpha[i];
							m_palette.Color(i, c);
						}
				}
			}
			else if ((type == PNG_COLOR_TYPE_GRAY_ALPHA) && (m_bitdepth <= 8)) {
				png_set_gray_to_rgb(m_png_ptr);
				png_set_add_alpha(m_png_ptr, 255, PNG_FILLER_AFTER);
				info.SurfaceFormat = Img::Format::ARGB8888;
			}
			else {
				return false;
			}

			m_passes = png_set_interlace_handling(m_png_ptr);
			if (m_passes < 1) {
				return false;
			}

			// Temp debug shit.
			png_read_update_info(m_png_ptr, m_info_ptr);
			png_get_rowbytes(m_png_ptr, m_info_ptr);

			return true;
		}
		catch (Err::CodecError&) {
			return false;
		}
	}

	AbstractCodec::AllocationStatus CodecPNG::PerformAllocate() {
		GetSurface()->CreateSurface(GetSize(), GetFormat());

		if (GetSurface()->GetFormat() == Format::Index8) {
			GetSurface()->SetPalette(m_palette);
		}

		return AllocationStatus::Ok;
	}

	AbstractCodec::LoadStatus CodecPNG::PerformLoadImageData(IO::FileReader::Ptr) {
		try {
			int height = GetSize().Height;
			int width = GetSize().Width;

			while (DoTerminate() == false) {
				if (m_currPass == m_passes) return LoadStatus::Finished;

				int nRows	= std::min<int>(ChunkRows, height - m_currScan);

				std::shared_ptr<Surface::LockedArea> area = GetSurface()->LockSurface(RectInt(PointInt(0, m_currScan), SizeInt(width, nRows)));
				uint8_t* pDst		= area->Buffer();
				size_t stride	= area->Stride();

				size_t dst_ofs	= 0;

				for(int i = m_currScan; i < (m_currScan + nRows); i++) {
					png_read_row(m_png_ptr, &pDst[dst_ofs], 0);
					dst_ofs += stride;
				}

				area->Unlock();

				m_currScan += nRows;

				if (m_currScan == height) {
					m_currPass++;
					m_currScan = 0;

					if (m_currPass == m_passes) {
						destroy_png();
						return LoadStatus::Finished;
					}
				}
			}
			return LoadStatus::Aborted;
		}
		catch (Err::CodecError&) {
			destroy_png();
			return LoadStatus::Failed;
		}
	}

	CodecPNG::CodecPNG():
		m_bitdepth(0),
		m_chans(0),
		m_png_ptr(0),
		m_info_ptr(0),
		m_currScan(0),
		m_currPass(0)
	{}

	CodecPNG::~CodecPNG() {
		try
		{
			destroy_png();
		}
		catch (...) {}
	}

	void CodecPNG::destroy_png() {
		try {
			if (m_png_ptr && m_info_ptr) {
				png_destroy_read_struct(&m_png_ptr, &m_info_ptr, 0);
				m_png_ptr	= 0;
				m_info_ptr	= 0;
			}
		}
		catch(Err::CodecError& ex) {
			Log << "(CodecPNG::destroy_png) " << ex.what() << "\n";
			throw;
		}
	}

	void CodecPNG::libpng_user_read_data( png_structp png_ptr, png_bytep data, png_size_t length ) {
		IO::FileReader* file = reinterpret_cast<IO::FileReader*>(png_get_io_ptr(png_ptr));
		file->ReadFull(data, length);
	}
}
