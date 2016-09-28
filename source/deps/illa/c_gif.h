#ifndef C_GIF_H
#define C_GIF_H

#include "codec.h"
#include "types.h"
#include "gif_stringtable.h"
#include "imagecomposer_gif.h"

namespace Img {
	class CodecGIF:public AbstractCodec {
	public:
		ImageComposer::Ptr RequestImageComposer() override;
		size_t FrameLoading();
		Palette GlobalPalette();
		bool HasGlobalPalette();

		CodecGIF();

	private:
		bool PerformLoadHeader(IO::FileReader::Ptr file, ImageInfo& info) override;

		void loadLogicalScreenDescriptor(ImageInfo &info, IO::FileReader::Ptr file);

		AllocationStatus PerformAllocate() override;
		LoadStatus PerformLoadImageData(IO::FileReader::Ptr file) override;
		size_t PerformEstimateMemory() override;

	private:
		enum {
			// Header packed masks.
			Mask_Header_GCTExist	= 0x80,
			Mask_Header_ColorRes	= 0x70,
			Mask_Header_Sort		= 0x08,
			Mask_Header_GCTSize		= 0x07,

			// Block types.
			Block_Extension			= 0x21,
			Block_ImageDescriptor	= 0x2C,
			Block_Trailer			= 0x3B, // Not really a block. Indicates EOF.

			// Extension blocks.
			Block_Ext_PlainText		= 0x01,
			Block_Ext_GraphicControl= 0xf9,
			Block_Ext_Comment		= 0xfe,
			Block_Ext_Application	= 0xff,
		};

	private:
		ImageComposerGif::Ptr m_composer;

		// Image data
		DescriptorVectorPtr m_imageDescriptors;

		// Global color table data
		bool m_gctExist = false;
		uint8_t m_colorRes = 0;
		bool m_gctSorted = false;
		int m_gctSize = 0;
		Img::Palette m_gctEntries;
		uint8_t m_bgrColor = 0;

		// Loading state
		size_t m_currentImage = 0;
		std::unique_ptr<StringTable> m_pCurrTable;

	};
}

#endif
