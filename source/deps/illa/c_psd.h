#ifndef C_PSD_H
#define C_PSD_H

#include "codec_static.h"
#include "psd_header.h"

namespace Img {
	class CodecPSD:public StaticCodec {
	public:

	private:
		bool PerformLoadHeader(IO::FileReader::Ptr file, ImageInfo& info) override;
		AllocationStatus PerformAllocate() override;
		LoadStatus PerformLoadImageData(IO::FileReader::Ptr file) override;

		enum {
			ScansPerChunk = 64
		};

		enum PSCompressionMode {
			CompressionRaw = 0,
			CompressionRle = 1,
			CompressionZipNoPrediction = 2,
			CompressionZipPrediction = 3
		};

		Internal::PsdHeader m_header;
		Img::Palette m_palette;

		uint16_t m_compression;
		int m_currentScanLine;
		int m_currentChannel;

		FileInt m_imageDataOffset;
		FileInt m_colorPlaneSize;

		typedef std::vector<uint16_t> RleScanlineSize;
		RleScanlineSize m_rleScanlineLength;

		typedef std::vector<signed char> RleStream;
		RleStream m_currentRleStreamData;
		int m_currentRleStreamIndex;
		int m_numRleEntries;

		Img::Format determineSurfaceFormat();

		AbstractCodec::LoadStatus loadRgb( IO::FileReader::Ptr file );

		uint8_t* transferIndexedScanline( uint8_t* destination, uint8_t* source, size_t numToTransfer );
		uint8_t* transferRgbScanlinePlane( uint8_t* destination, uint8_t* source, size_t numToTransfer );
		uint8_t* transferBitmapScanline( uint8_t* destination, uint8_t* source, size_t numToTransfer );

		bool loadRawStream( IO::FileReader::Ptr file, uint8_t* dest, size_t numBytes );
		bool loadRleStream( IO::FileReader::Ptr file, uint8_t* dest, size_t numBytes );
	};
}

#endif
