#include "tiff_loader.h"

namespace Img {
	void TiffLoader::Create( TIFF* tiff, Surface::Ptr surface, TiffDataConverter::Ptr converter ) {
		m_tiff = tiff;
		m_surface = surface;
		m_converter = converter;

		PerformOnCreate();
	}

	TiffChunkStatus TiffLoader::LoadChunk() {
		return PerformLoadChunk();
	}

	TiffLoader::~TiffLoader() {}
}
