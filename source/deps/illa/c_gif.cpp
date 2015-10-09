#include "c_gif.h"
#include "imagecomposer_gif.h"
#include "surfacemgr.h"
#include "gif_io.h"
#include "config.h"

namespace Img {
	using namespace Geom;

	struct GIFInvalidHeader:public Err::InvalidHeader {
		GIFInvalidHeader(const std::string& msg):Err::InvalidHeader("GIF header: " + msg) {}
	};

	bool CodecGIF::PerformLoadHeader(IO::FileReader::Ptr file, ImageInfo& info) {
		char header[7] = { 0 };
		memset(header, 0, 7);
		file->ReadFull(header, 6);

		// Validate header (only GIF87a and GIF89a is valid).
		if (strncmp(header, "GIF87a", 6) && strncmp(header, "GIF89a", 6)) {
			return false;
		}

		loadLogicalScreenDescriptor(info, file);
		if (m_gctExist) {
			ReadPalette(m_gctEntries, m_gctSize, file);
		}

		// Load the image descriptors. One for non-animated images, some amount for animated.
		bool done = false;

		ImageDescriptor::Ptr pID;

		while(done == false) {
			uint8_t block;
			file->ReadFull(&block, 1);

			switch(block) {
				case Block_Extension: {
					if (pID == nullptr) {
						pID = std::make_shared<ImageDescriptor>();
					}
					uint8_t label;
					file->ReadFull(&label, 1);
					switch (label) {
						case Block_Ext_GraphicControl:
							pID->ReadGraphicControlExtBlock(file);
							break;
					}
					break;
				}

				case Block_ImageDescriptor: {
					if (pID == nullptr) {
						pID = std::make_shared<ImageDescriptor>();
					}
					pID->ReadDescriptor(file);

					m_imageDescriptors->push_back(pID);
					pID.reset();	// Stop using this image descriptor (no blocks modifying this one may follow)
					break;
				}
				case Block_Trailer:
				default:
					done = true;
					break;
			}
			if (!done) {
				uint8_t blockSize;
				do {
					if ((file->Read(&blockSize, 1, 1) == 0)) {
						// File seems to be cropped. Stop bothering
						done = true;
						break;
					}
					file->Seek(blockSize, IO::SeekMethod::Current);
				}
				while(blockSize != 0);
			}
		}


		if (m_imageDescriptors->size() == 0) {
			return false;
		}

		info.SurfaceFormat	= Img::Format::Index8;

		return true;
	}

	AbstractCodec::AllocationStatus CodecGIF::PerformAllocate() {
		for(auto id: *m_imageDescriptors) {
			id->Surface = CreateNewSurface();
			id->Surface->CreateSurface(id->ImageSize, Img::Format::Index8);
			id->Surface->ClearSurface(Img::Color(m_bgrColor, m_bgrColor, m_bgrColor, m_bgrColor));
		}
		m_composer->SendImageData(m_imageDescriptors, GetSize(), m_gctEntries);
		return AllocationStatus::Ok;
	}

	AbstractCodec::LoadStatus CodecGIF::PerformLoadImageData(IO::FileReader::Ptr file) {
		while (DoTerminate() == false) {
			auto currentID = (*m_imageDescriptors)[m_currentImage];
			if (currentID->Surface == nullptr) {
				DO_THROW(Err::InvalidCall, "Surface not created.");
			}

			// See if a new image should start loading
			if (m_pCurrTable.get() == 0) {
				// Yep, set it up
				file->Seek(currentID->FileOffset, IO::SeekMethod::Begin);

				uint8_t codeSize;
				file->ReadFull(&codeSize, 1);

				if (codeSize > 12) {
					return LoadStatus::Failed;
				}
				m_pCurrTable = std::make_unique<StringTable>(currentID, codeSize, currentID->Interlace);
			}

			// Load a chunk of the current image
			uint8_t left;	file->ReadFull(&left, 1);
			bool isDone=false;
			if (left > 0) {
				uint8_t pCurrBuffer[255];
				size_t read = file->Read(pCurrBuffer, 1, left); // Load into buffer
				isDone=!m_pCurrTable->Decompress(pCurrBuffer, read);
				if (left != read) {
					isDone = true; // File is probably cropped.
				}
			}
			else {
				isDone = true;
			}

			if (isDone) {
				// Current image is done, start loading a new one next pass
				m_pCurrTable.reset();
				m_currentImage++;
				m_composer->FrameFinished(m_currentImage);

				if (m_currentImage == m_imageDescriptors->size()) return LoadStatus::Finished;
			}
		}
		return LoadStatus::Aborted;
	}

	size_t CodecGIF::PerformEstimateMemory() {
		return m_imageDescriptors->size() * Area(GetSize());
	}

	CodecGIF::CodecGIF():
		m_currentImage{ 0 },
		m_composer{ std::make_shared<ImageComposerGif>() },
		m_imageDescriptors{ std::make_shared<DescriptorVector>() }
	{}

	ImageComposer::Ptr CodecGIF::RequestImageComposer() {
		return m_composer;
	}

	void CodecGIF::loadLogicalScreenDescriptor( ImageInfo &info, IO::FileReader::Ptr file ) {
		ReadSizeWord(&info.Dimensions, file);

		uint8_t packed;
		file->ReadFull(&packed, 1);
		file->ReadFull(&m_bgrColor, 1);

		uint8_t aspectRatio;
		file->ReadFull(&aspectRatio, 1);

		m_gctExist	= (packed & Mask_Header_GCTExist) != 0;
		m_colorRes	= static_cast<uint8_t>((2 << ((0xff & ((packed & Mask_Header_ColorRes) >> 4)) - 1)) - 1);
		m_gctSorted	= (packed & Mask_Header_Sort) != 0;
		m_gctSize	= static_cast<int>(2 << (0xff & (packed & Mask_Header_GCTSize)));
	}

	size_t CodecGIF::FrameLoading() {
		return m_currentImage;
	}

	Img::Palette CodecGIF::GlobalPalette() {
		return m_gctEntries;
	}

	bool CodecGIF::HasGlobalPalette() {
		return m_gctExist;
	}
}
