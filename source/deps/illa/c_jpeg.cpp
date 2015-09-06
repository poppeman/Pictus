#include "StdAfx.h"
#include "c_jpeg.h"
#include "surface_locked_area.h"

namespace Img {
	using namespace Geom;

	void CodecJPEG::jpeg_error_exit(j_common_ptr cinfo) {
		cinfo; // Not used
		//our_error* err=(our_error*)cinfo->err;
		// TODO: Try to extract information from cinfo
		DO_THROW(Err::CodecError, TX("Error loading JPEG."));
	}

	bool CodecJPEG::PerformLoadHeader(IO::FileReader::Ptr file, ImageInfo& info) {
		try {
			m_file = file;
			PrepareLibJpeg(file);

			info.SurfaceFormat = Img::Format::Undefined;
			if (m_decInfo.num_components == 3 || m_decInfo.num_components == 4) {
				info.SurfaceFormat = Img::Format::XRGB8888;
			}
			else if (m_decInfo.num_components == 1) {
				info.SurfaceFormat = Img::Format::Index8;
			}
			else {
				return false;
			}

			info.Dimensions = SizeInt(m_decInfo.image_width, m_decInfo.image_height);

			return true;
		}
		catch (Err::CodecError&) {
			destroy();
			return false;
		}
	}

	AbstractCodec::AllocationStatus CodecJPEG::PerformAllocate(const Geom::SizeInt& dimHint) {
		if (m_isError) {
			return AllocationStatus::Failed;
		}
		m_decInfo.scale_num = 1;
		m_decInfo.scale_denom = 1;

		Geom::SizeFloat scaleDenom = dimHint.StaticCast<float>() / GetSize().StaticCast<float>();
		float factor = Util::Max(scaleDenom.Width, scaleDenom.Height);
		if (factor < 0.5f) {
			m_decInfo.scale_denom = 2;
		}
		if (factor < 0.25f) {
			m_decInfo.scale_denom = 4;
		}
		if (factor < 0.125f) {
			m_decInfo.scale_denom = 8;
		}
		if (factor >= 1.0f) {
			return AllocationStatus::NotSupported;
		}

		UseFastSurface(true);

		return CreateSurface();
	}

	AbstractCodec::AllocationStatus CodecJPEG::PerformAllocate() {
		if (m_isError) {
			return AllocationStatus::Failed;
		}

		UseFastSurface(false);

		return CreateSurface();
	}


	AbstractCodec::AllocationStatus CodecJPEG::CreateSurface() {
		if (m_isError) {
			return AllocationStatus::Failed;
		}

		if (m_decInfo.global_state == 0) {
			PrepareLibJpeg(m_file);
		}

		try {
			jpeg_start_decompress(&m_decInfo);
		}
		catch(Err::Exception& e) {
			return AllocationStatus::Failed;
		}

		GetSurface()->CreateSurface(Geom::SizeInt(m_decInfo.output_width, m_decInfo.output_height), GetFormat());
		GetSurface()->ClearSurface(Img::Color(0xff, 0xff, 0xff, 0xff));

		if (m_decInfo.output_components == 1) {
			GetSurface()->SetPalette(Grayscale());
		}

		for (int i = 0; i < ChunkRows; i++) {
			m_pRow[i] = new JSAMPLE[GetSurface()->Width() * 4];
		}

		return AllocationStatus::Ok;
	}

	AbstractCodec::LoadStatus CodecJPEG::PerformLoadImageData(IO::FileReader::Ptr) {
		try {
			while (DoTerminate() == false) {
				if (m_decInfo.output_scanline >= m_decInfo.output_height) {
					FinalizeSurface();
					destroy();
					return LoadStatus::Finished;
				}

				int pSize = GetSurface()->PixelSize();

				// Load up to ChunkRows scan-lines
				int loaded = 0;
				while(loaded < ChunkRows) {
					if (m_decInfo.output_scanline >= m_decInfo.output_height) {
						break;
					}

					Geom::PointInt tlToLock = { 0, static_cast<int>(m_decInfo.output_scanline) };

					int num = jpeg_read_scanlines(&m_decInfo, m_pRow, ChunkRows);
					if (num == 0) {
						break;
					}

					loaded += num;

					Surface::Ptr surface(GetSurface());
					std::shared_ptr<Surface::LockedArea> area = surface->LockSurface(RectInt(tlToLock, SizeInt(surface->GetSize().Width, num)));
					uint8_t* pDst = area->Buffer();

					for(int row = 0; row < num; row++) {
						size_t yofs = row * area->Stride();

						int dstofs = m_decInfo.output_components - 1;
						if(m_decInfo.output_components == 4)
							dstofs-=1;

						// Have to flip RGB<->BGR
						int width = surface->Width();

						for(int pix = 0; pix < width; pix++) {
							for (int field = 0; field < m_decInfo.output_components; field++) {
								pDst[yofs + field] = m_pRow[row][dstofs - field];
							}

							yofs	+= pSize;
							dstofs	+= m_decInfo.output_components;
						}
					}
				}
			}

			return LoadStatus::Aborted;
		}
		catch (Err::CodecError&) {
			destroy();
			return LoadStatus::Failed;
		}
	}

	CodecJPEG::CodecJPEG():m_isInit(false), m_isError(false) {
		for (int i = 0; i < ChunkRows; i++) {
			m_pRow[i] = 0;
		}
	}

	CodecJPEG::~CodecJPEG() {
		destroy();
	}

	void CodecJPEG::destroy() {
		if (m_isInit && !IsFinished()) {
			jpeg_abort_decompress(&m_decInfo);
			jpeg_destroy_decompress(&m_decInfo);
		}
		else if (m_isInit) {
			if (m_decInfo.output_scanline) {
				jpeg_finish_decompress(&m_decInfo);
			}

			jpeg_destroy_decompress(&m_decInfo);
		}
		m_decompressionSource.file.reset();
		m_isInit = false;
		for(int i = 0; i < ChunkRows; i++) {
			if (m_pRow[i]) {
				delete[] m_pRow[i];
				m_pRow[i] = 0;
			}
		}
	}

	boolean CodecJPEG::fill_input_buffer( j_decompress_ptr cinfo ) {
		SourceModule* src = reinterpret_cast<SourceModule*>(cinfo->src);
		FileInt numRead = src->file->Read(src->buffer, 1, BytesPerBuffer);
		if (numRead == 0) {
			src->buffer[0] = 0xff;
			src->buffer[1] = JPEG_EOI;
			numRead = 2;
		}
		src->pub.bytes_in_buffer = static_cast<size_t>(numRead);
		src->pub.next_input_byte = src->buffer;

		return TRUE;
	}

	void CodecJPEG::skip_input_data( j_decompress_ptr cinfo, long num_bytes ) {
		SourceModule* src = reinterpret_cast<SourceModule*>(cinfo->src);

		if (num_bytes > 0) {
			while (num_bytes > (long) src->pub.bytes_in_buffer) {
				num_bytes -= (long) src->pub.bytes_in_buffer;
				(void) CodecJPEG::fill_input_buffer(cinfo);
			}
			src->pub.next_input_byte += (size_t) num_bytes;
			src->pub.bytes_in_buffer -= (size_t) num_bytes;
		}

	}

	void CodecJPEG::term_source( j_decompress_ptr cinfo ) {
		SourceModule* src = reinterpret_cast<SourceModule*>(cinfo->src);
		src->file.reset();
	}

	void CodecJPEG::init_source(j_decompress_ptr) {}

	void CodecJPEG::PrepareLibJpeg( IO::FileReader::Ptr file ) {
		file->Seek(0, IO::SeekMethod::Begin);
		m_decompressionSource.pub.fill_input_buffer	= fill_input_buffer;
		m_decompressionSource.pub.resync_to_restart	= jpeg_resync_to_restart;
		m_decompressionSource.pub.skip_input_data	= skip_input_data;
		m_decompressionSource.pub.term_source		= term_source;
		m_decompressionSource.pub.init_source		= init_source;
		m_decompressionSource.file = file;
		m_decompressionSource.pub.bytes_in_buffer = 0;
		m_decompressionSource.pub.next_input_byte = 0;

		m_decInfo.err	= jpeg_std_error(&m_decErr.pub);
		m_decErr.pCodec	= this;
		m_decErr.pub.error_exit = jpeg_error_exit;

		jpeg_create_decompress(&m_decInfo);
		m_isInit = true;

		m_decInfo.src = &m_decompressionSource.pub;

		jpeg_read_header(&m_decInfo, TRUE);

		m_decInfo.err->trace_level	= 0;
		m_decInfo.two_pass_quantize = false;
		m_decInfo.quantize_colors	= false;
		m_decInfo.do_fancy_upsampling = false;
	}
}
