#ifndef C_PSP_H
#define C_PSP_H

#include "orz/file_cache.h"

#include "codec_static.h"
#include "psp_defines.h"

namespace Img {
	class CodecPSP:public StaticCodec {
	public:
		enum {
			ScansPerChunk = 16
		};

		CodecPSP();

	private:
		bool PerformLoadHeader(IO::FileReader::Ptr file, ImageInfo& info) override;
		AllocationStatus PerformAllocate() override;
		LoadStatus PerformLoadImageData(IO::FileReader::Ptr file) override;

		static const uint8_t FileHeader[32];
		static const uint8_t Gen_HeaderID[4];

		struct BlockHeader {
			PSPBlockID ID;
			uint32_t Length;

			FileInt PositionStart;
			FileInt PositionEnd;
		};

		struct Layer {
			// String Name;
			PSPLayerType Type;
			//Geom::RectInt LayerArea;
			Geom::RectInt SavedLayerArea;
			//uint8_t					Opacity;
			PSPBlendModes BlendMode;
			uint8_t LayerFlags;
			// uint8_t TransparencyProtected;
			// uint8_t LinkGroup;
			// Geom::RectInt MaskArea;
			// Geom::RectInt SavedMaskArea;
			// uint8_t IsMaskLinked;
			// uint8_t IsMaskDisabled;
			// uint8_t InvertMaskOnBlend;
			// uint16_t BlendRangeCount;
			// uint8_t SourceBlendRange1[4];
			// uint8_t DestinationBlendRange1[4];
			// uint8_t SourceBlendRange2[4];
			// uint8_t DestinationBlendRange2[4];
			// uint8_t SourceBlendRange3[4];
			// uint8_t DestinationBlendRange3[4];
			// uint8_t SourceBlendRange4[4];
			// uint8_t DestinationBlendRange4[4];
			// uint8_t SourceBlendRange5[4];
			// uint8_t DestinationBlendRange5[4];
			// uint8_t UseHighlightColor;
			// uint32_t HighlightColor;
		};

		struct Channel {
			uint32_t SizeCompressed;
			uint32_t SizeUncompressed;
			PSPDIBType TypeDIB;
			PSPChannelType TypeChannel;
			FileInt PositionData;

			FileInt CurrentSourcePosition;
		};

		bool ReadBlockHeader(IO::FileReader::Ptr reader, BlockHeader* outHeader);

		bool ReadLayerBlock(IO::FileReader::Ptr reader, Layer* outLayer);
		bool ReadChannelBlock(IO::FileReader::Ptr reader, Channel* outChannel);

		void ParseFileStructure(IO::FileReader::Ptr reader);

		bool DecodeScanlines(IO::FileReader::Ptr reader, int scansToProcess, int width, size_t stride, size_t pixelSize, uint8_t* destination);

		bool DecodeRawScanlines(IO::FileReader::Ptr reader, int scansToProcess, int width, size_t stride, size_t pixelSize, uint8_t* destination);
		bool DecodeRLEScanlines(IO::FileReader::Ptr reader, int scansToProcess, int width, size_t stride, size_t pixelSize, uint8_t* destination);

		bool DecodeRLEChannel(IO::FileReader::Ptr reader, int scansToProcess, int width, size_t stride, size_t pixelSize, uint8_t* destination);

		Layer m_layer;

		uint8_t m_bitDepth;

		typedef std::vector<Channel> ChannelVector;

		ChannelVector m_channels;

		PSPCompression m_compression;
		bool m_isStructureParsed;

		int m_currScanline;

		IO::FileReaderByteStreamer m_fileStream;
	};
}

#endif
