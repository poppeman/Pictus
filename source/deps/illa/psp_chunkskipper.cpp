#include "psp_chunkskipper.h"
#include "orz/logger.h"

namespace Img {
	namespace Internal {
		ChunkSkipper::ChunkSkipper(IO::FileReader::Ptr reader) :m_reader(reader) {
			FileInt pos = reader->Position();
			uint32_t chunkSize;
			reader->ReadFull(&chunkSize, 4);

			m_end = pos + chunkSize;
		}

		ChunkSkipper::~ChunkSkipper() {
			try
			{
				m_reader->Seek(m_end, IO::SeekMethod::Begin);
			}
			catch (std::exception& e)
			{
				Log << "(ChunkSkipper::~ChunkSkipper) " << e.what() << "\n";
			}
		}
	}
}
