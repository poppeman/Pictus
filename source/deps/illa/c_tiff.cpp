#include "c_tiff.h"
#include "tiff_striploader.h"
#include "tiff_tileloader.h"
#include "tiff_cieconverter.h"
#include "tiff_rawconverter.h"
#include "tiff_ycbcrconverter.h"

#include "orz/logger.h"

struct InternalTIFFException:public Err::Exception {
	InternalTIFFException():
		Err::Exception("numpty")
	{}
};

std::string UnTIFFifyParams(const char *module, const char *fmt, va_list ap)
{
	char tmp[2];

	// We don't actually think that the string will fit in 2 chars, but Microsoft likes to pretend that passing null to
	// vsnprintf is an error so we dodge that to prevent hilarity.
	va_list args;
	va_copy(args, ap);
	auto len = vsnprintf(tmp, 2, fmt, args);
	va_end(args);

	std::string str(len + 1, 0);
	vsnprintf(&str[0], len + 1, fmt, ap);
	str.resize(str.length() - 1);

	std::stringstream ss;
	ss << module << " " << str;
	return ss.str();
}

void myTIFFErrorHandler(const char *module, const char *fmt, va_list ap) {
	Log << "(CodecTIFF:myTIFFErrorHandler) " << UnTIFFifyParams(module, fmt, ap) << "\n";
	// TODO: Not sure if libtiff likes exceptions here (probably not) so should signal an error in a safer way
}

void myTIFFWarningHandler(const char *module, const char *fmt, va_list ap) {
	Log << "(CodecTIFF:myTIFFWarningHandler) " << UnTIFFifyParams(module, fmt, ap) << "\n";
}

tsize_t myTIFFReadProc(thandle_t handle, tdata_t data, tsize_t size) {
	IO::FileReader* reader = reinterpret_cast<IO::FileReader*>(handle);
	return static_cast<tsize_t>(reader->Read(data, 1, size));
}

tsize_t myTIFFWriteProc(thandle_t, tdata_t, tsize_t) {
	DO_THROW(Err::Unsupported, "Attempted to write.");
}

int myTIFFMapFileProc(thandle_t, tdata_t*, toff_t*) {
	DO_THROW(Err::Unsupported, "Mapping not supported.");
}

void myTIFFUnmapFileProc(thandle_t, tdata_t, toff_t) {
	DO_THROW(Err::Unsupported, "Unmapping not supported.");
}

static toff_t myTIFFSeekProc(thandle_t handle, toff_t offset, int whence) {
	auto* reader = reinterpret_cast<IO::FileReader*>(handle);
	IO::SeekMethod m;
	switch(whence) {
		case SEEK_SET:
			m = IO::SeekMethod::Begin;
			break;
		case SEEK_CUR:
			m = IO::SeekMethod::Current;
			break;
		case SEEK_END:
			m = IO::SeekMethod::End;
			break;
		default:
			DO_THROW(Err::InvalidParam, "Invalid whence: " + ToAString(whence));
	}
	reader->Seek(offset, m);
	return static_cast<toff_t>(reader->Position());
}

static toff_t myTIFFSizeProc(thandle_t handle) {
	IO::FileReader* reader = reinterpret_cast<IO::FileReader*>(handle);
	return static_cast<toff_t>(reader->Size());
}

static int myTIFFCloseProc(thandle_t) {
	// Let's not and say we did
	return 0;
}

namespace Img {
	bool CodecTIFF::PerformLoadHeader(IO::FileReader::Ptr file, ImageInfo& info) {
		try {
			TIFFSetErrorHandler(myTIFFErrorHandler);
			TIFFSetWarningHandler(myTIFFWarningHandler);

			m_tiff = TIFFClientOpen(file->Name().c_str(), "rm", reinterpret_cast<thandle_t>(file.get()), myTIFFReadProc, myTIFFWriteProc, myTIFFSeekProc, myTIFFCloseProc, myTIFFSizeProc, myTIFFMapFileProc, myTIFFUnmapFileProc);

			if (!m_tiff) {
				return false;
			}

			int isTiled = TIFFIsTiled(m_tiff);
			if (isTiled == 0) {
				m_loader = std::make_shared<TiffStripLoader>();
			}
			else {
				m_loader = std::make_shared<TiffTileLoader>();
			}

			uint32_t w, h;
			TIFFGetField(m_tiff, TIFFTAG_IMAGEWIDTH, &w);
			TIFFGetField(m_tiff, TIFFTAG_IMAGELENGTH, &h);
			info.Dimensions		= Geom::SizeInt(w, h);

			uint16_t planar;
			TIFFGetField(m_tiff, TIFFTAG_PLANARCONFIG, &planar);

			// FIXME: Add separate color planes support
			if (planar == PLANARCONFIG_SEPARATE) {
				return false;
			}

			uint16_t spp;
			TIFFGetField(m_tiff, TIFFTAG_SAMPLESPERPIXEL, &spp);

			uint16_t bitsPerSample;
			TIFFGetField(m_tiff, TIFFTAG_BITSPERSAMPLE, &bitsPerSample);

			m_photometric = 1;
			TIFFGetField(m_tiff, TIFFTAG_PHOTOMETRIC, &m_photometric);

			if (m_photometric == PHOTOMETRIC_YCBCR) {
				m_converter = std::make_shared<TiffYCbCrConverter>();
			}
			/*else if (m_photometric == PHOTOMETRIC_CIELAB || m_photometric == PHOTOMETRIC_LOGL)
			{
				m_converter.reset(new TiffCieConverter);
			}*/
			else if (m_photometric == PHOTOMETRIC_RGB || m_photometric == PHOTOMETRIC_PALETTE || m_photometric == PHOTOMETRIC_MINISBLACK || m_photometric == PHOTOMETRIC_MINISWHITE) {
				m_converter = std::make_shared<TiffRawConverter>();
			}
			else {
				return false;
			}

			uint16_t compression;
			TIFFGetField(m_tiff, TIFFTAG_COMPRESSION, &compression);

			if (spp == 1) {
				uint16_t colorCount = 1 << std::min<uint16_t>(bitsPerSample, 8);

				if (m_photometric == PHOTOMETRIC_PALETTE && (bitsPerSample >= 1 && bitsPerSample <= 8)) {
					info.SurfaceFormat = Img::Format::Index8;

					uint16_t* red;
					uint16_t* green;
					uint16_t* blue;

					if (TIFFGetField(m_tiff, TIFFTAG_COLORMAP, &red, &green, &blue) == 0) {
						return false;
					}

					for (int color = 0; color < colorCount; ++color) {
						m_palette.Color(
							color,
							Img::Color(0xff,
							static_cast<uint8_t>(red[color] >> 8),
							static_cast<uint8_t>(green[color] >> 8),
							static_cast<uint8_t>(blue[color] >> 8)));
					}
				}
				else if ((bitsPerSample >= 1 && bitsPerSample <= 8) || bitsPerSample == 16) {
					if (m_photometric == PHOTOMETRIC_MINISBLACK) {
						info.SurfaceFormat = Img::Format::Index8;
						m_palette = Grayscale(0, colorCount - 1);
					}
					else if (m_photometric == PHOTOMETRIC_MINISWHITE) {
						info.SurfaceFormat = Img::Format::Index8;
						m_palette = Grayscale(colorCount - 1, 0);
					}
				}
			}
			else if (spp == 3 && bitsPerSample == 8) {
				info.SurfaceFormat = Img::Format::XRGB8888;
			}
			else if (spp == 4 && bitsPerSample == 8) {
				uint16_t numExtraSample		= 0;
				uint16_t* typeExtraSample	= 0;
				TIFFGetField(m_tiff, TIFFTAG_EXTRASAMPLES, &numExtraSample, &typeExtraSample);

				info.SurfaceFormat = numExtraSample > 0?Img::Format::ARGB8888:Img::Format::XRGB8888;
			}
	
			if (info.SurfaceFormat == Img::Format::Undefined) {
				return false;
			}

			return true;
		}
		catch (InternalTIFFException&) {
			return false;
		}
	}

	AbstractCodec::AllocationStatus CodecTIFF::PerformAllocate() {
		GetSurface()->CreateSurface(GetSize(), GetFormat());
		if (GetSurface()->GetFormat() == Img::Format::Index8)
			GetSurface()->SetPalette(m_palette);

		m_converter->Create(m_tiff, GetSurface()->GetFormat());
		m_loader->Create(m_tiff, GetSurface(), m_converter);

		return AllocationStatus::Ok;
	}

	AbstractCodec::LoadStatus CodecTIFF::PerformLoadImageData(IO::FileReader::Ptr) {
		try {
			while (DoTerminate() == false) {
				auto state = m_loader->LoadChunk();
				if (state == Img::TiffChunkStatus::Finished) {
					endImageIO();
					return LoadStatus::Finished;
				}
				else if (state == Img::TiffChunkStatus::Error) {
					endImageIO();
					return LoadStatus::Failed;
				}
			}
			return LoadStatus::Aborted;
		}
		catch (InternalTIFFException&) {
			return LoadStatus::Failed;
		}
	}

	CodecTIFF::CodecTIFF():
		m_tiff{ nullptr }
	{}

	CodecTIFF::~CodecTIFF() {
		endImageIO();
	}

	void CodecTIFF::endImageIO() {
		if (m_tiff) {
			TIFFClose(m_tiff);
		}

		m_tiff = nullptr;
	}
}
