#ifndef PSP_CHUNKSKIPPER_H
#define PSP_CHUNKSKIPPER_H

#include "orz/file_reader.h"

namespace Img {
	namespace Internal {
		class ChunkSkipper {
		public:
			explicit ChunkSkipper(IO::FileReader::Ptr reader);
			~ChunkSkipper();

		private:
			ChunkSkipper(ChunkSkipper& rhs);
			ChunkSkipper& operator= (const ChunkSkipper& rhs);

			FileInt					m_end;
			IO::FileReader::Ptr		m_reader;
		};
	}
}

#endif
