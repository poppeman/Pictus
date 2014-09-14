#include "StdAfx.h"
#include "filterjob.h"
#include "filter_int.h"

namespace Filter {
	namespace Internal {
		using namespace Geom;

		Filter::Mode FilterJob::m_mode;
		const FilterBuffer* FilterJob::m_source;
		Img::Format FilterJob::m_format;
		float FilterJob::m_zoom;

		void FilterJob::Construct(int chunk, const FilterBuffer& dest, const Geom::RectInt& croppedRegion) {
			m_our_mode = m_mode;
			m_dest = dest;
			m_croppedRegion = (chunk==-1)?(croppedRegion):RectInt(croppedRegion.TopLeft() + chunk * SizeInt(0, ChunkSize), SizeInt(croppedRegion.Width(), ChunkSize));

			if (chunk == -1) {
				return;
			}
			if (m_croppedRegion.Bottom() > croppedRegion.Bottom()) {
				m_croppedRegion.Bottom(croppedRegion.Bottom());
			}
			if (m_croppedRegion.Height() <= 0) {
				m_our_mode = Mode::Undefined;
			}
			size_t num_dest_rows = chunk * ChunkSize;
			size_t dest_offset = m_dest.Stride * num_dest_rows;
			m_dest.BufferData += dest_offset;
			m_dest.Dimensions.Height -= num_dest_rows;
		}

		void FilterJob::SetSource(const Filter::Mode mode, const FilterBuffer* source, Img::Format format, float zoom) {
			m_mode = mode;
			m_source = source;
			m_zoom = zoom;
			m_format = format;
		}

		void FilterJob::Run() {
			switch (m_mode) {
			case Mode::Bilinear:
				Scale::Bilinear(*m_source, m_dest, m_croppedRegion, m_format, m_zoom);
				break;
			case Mode::NearestNeighbor:
				Scale::NearestNeighbor(*m_source, m_dest, m_croppedRegion, m_format, m_zoom);
				break;
			case Mode::Lanczos3:
				Scale::Lanczos3(*m_source, m_dest, m_croppedRegion, m_format, m_zoom);
				break;
			default:
				break;
			}
		}
	}
}
