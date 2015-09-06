#include "StdAfx.h"
#include "c_xyz.h"
#include "surface.h"
#include "surface_locked_area.h"
#include "surfacemgr.h"

namespace Img {
	using namespace Geom;

	CodecXYZ::CodecXYZ():
		m_isInit{ false },
		m_x{ 0 },
		m_y{ 0 },
		m_paletteSet{ false }
	{
		memset(&m_stream, 0, sizeof(m_stream));
	}

	bool CodecXYZ::PerformLoadHeader( IO::FileReader::Ptr file, ImageInfo& info ) {
		char header[4];
		if (file->Size() < 10) {
			return false;
		}
		file->ReadFull(header, 4);
		if (memcmp(header, "XYZ1", 4) != 0) {
			Log << L"(CodecXYZ::LoadHeader) XYZ1 header missing.\n";
			return false;
		}
		uint16_t width, height;
		file->ReadFull(&width, 2);
		file->ReadFull(&height, 2);

		m_size = Geom::SizeInt(width, height);
		info.Dimensions = m_size;
		info.SurfaceFormat = Img::Format::Index8;

		memset(&m_stream, 0, sizeof(m_stream));
		m_stream.next_in = m_inBuffer;

		if (inflateInit(&m_stream) != Z_OK) {
			// TODO: Log error, zlib failing is not intended behavior
			Log << L"(CodecXYZ::LoadHeader) Failed initializing zlib.\n";
			return false;
		}
		m_isInit = true;

		return true;
	}

	AbstractCodec::AllocationStatus CodecXYZ::PerformAllocate() {
		GetSurface()->CreateSurface(m_size, Img::Format::Index8);
		return AllocationStatus::Ok;
	}

	AbstractCodec::LoadStatus CodecXYZ::PerformLoadImageData( IO::FileReader::Ptr file ) {
		while(DoTerminate() == false) {
			FillBuffer(file);
			if (m_outData.empty()) {
				Cleanup();
				Log << L"(CodecXYZ::LoadHeader) Prematurely encountered end of file.\n";
				return LoadStatus::Failed;
			}

			if (m_paletteSet == false && m_outData.size() >= 768) {
				m_paletteSet = true;
				Img::Palette pal;
				for (int i = 0; i < 256; ++i) {
					pal.Color(i, Img::Color(0xff, m_outData[0], m_outData[1], m_outData[2]));
					m_outData.pop_front();
					m_outData.pop_front();
					m_outData.pop_front();
				}
				GetSurface()->SetPalette(pal);
			}

			if (m_paletteSet) {
				Img::Surface::LockedArea::Ptr area = GetSurface()->LockSurface(
					RectInt(
						PointInt(0, m_y),
						SizeInt(m_size.Width, m_size.Height - m_y)));

				int currY = 0;
				uint8_t* buf = area->Buffer();
				size_t stride = area->Stride();
				while (m_outData.size() > 0) {
					buf[currY * stride + m_x] = m_outData.front();
					m_outData.pop_front();
					m_x++;
					if (m_x == m_size.Width) {
						m_x = 0;
						m_y++;
						currY++;

						if (m_y == m_size.Height) {
							Cleanup();
							return LoadStatus::Finished;
						}
					}
				}
			}
		}
		return LoadStatus::Aborted;
	}

	bool CodecXYZ::FillBuffer(IO::FileReader::Ptr file) {
		uint8_t in_buffer[InBufferSize];
		uint8_t out_buffer[OutBufferSize];

		auto inRemaining = file->Read(in_buffer, 1, InBufferSize);
		m_stream.next_in = in_buffer;
		m_stream.avail_in = inRemaining;

		do {
			m_stream.next_out = out_buffer;
			m_stream.avail_out = OutBufferSize;
			auto ret = inflate(&m_stream, 0);
			if (ret != Z_OK && ret != Z_STREAM_END) {
				Cleanup();
				return false;
			}
			int num_decompressed = OutBufferSize - m_stream.avail_out;
			for (int i = 0; i < num_decompressed; ++i) {
				m_outData.push_back(out_buffer[i]);
			}
		} while (m_stream.avail_in);

		return true;
	}

	void CodecXYZ::Cleanup() {
		if (m_isInit) {
			inflateEnd(&m_stream);
		}
		m_isInit = false;
	}

	CodecXYZ::~CodecXYZ() {
		Cleanup();
	}
}
