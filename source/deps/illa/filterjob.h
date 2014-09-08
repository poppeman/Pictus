#ifndef FILTERJOB_H
#define FILTERJOB_H

#include "types.h"

namespace Filter {
	namespace Internal {
		class FilterJob {
		public:
			static const int ChunkSize = 256;

			void Construct(int chunk, const FilterBuffer& dest, const Geom::RectInt& croppedRegion);
			static void SetSource(const FilterMode mode, const FilterBuffer* source, Img::Format format, float zoom);

			void Run();
			void operator()();

		private:
			FilterJob& operator=(const FilterJob& rhs);

			FilterMode m_our_mode;
			static FilterMode m_mode;
			static const FilterBuffer* m_source;
			static Img::Format m_format;
			static float m_zoom;

			FilterBuffer m_dest;
			Geom::RectInt m_croppedRegion;
		};
	}
}

#endif
