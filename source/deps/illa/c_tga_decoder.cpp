#include "c_tga_decoder.h"
#include "surface_locked_area.h"

namespace Img {
	using namespace Geom;

	void CodecTgaDecoder::SetSource( IO::FileReader::Ptr reader, const TGAHeader* header ) {
		m_reader = reader;
		m_header = header;

		m_currentScanline = m_header->FlipVertical?m_header->Size.Height:0;

		OnSource();
	}

	void CodecTgaDecoder::SetDestination( Img::Surface::Ptr surfaceToProcess ) {
		m_surface = surfaceToProcess;
	}

	bool CodecTgaDecoder::Process() {
		if (m_reader == 0) {
			DO_THROW(Err::CriticalError, "reader not set, called Process() before Create()");
		}
		if (m_surface == 0) {
			DO_THROW(Err::CriticalError, "surface not set, called Process() before Create()");
		}

		Batch batch = PrepareBatch(RowsPerBatch);

		// OnProcess MAY modify batch->Destination and the original value MAY be needed inside FinalizeBatch.
		uint8_t* originalDestinationPointer = batch.Destination;
		bool isFinishedProcess = OnProcess(&batch);
		batch.Destination = originalDestinationPointer;

		bool isFinishedFinalized = FinalizeBatch(&batch);
		return isFinishedFinalized || isFinishedProcess;
	}

	void CodecTgaDecoder::OnSource() {}

	CodecTgaDecoder::Batch CodecTgaDecoder::PrepareBatch( int rowsToProcess ) {
		Batch batch;

		if (m_header->FlipVertical) {
			batch.NumRows	= std::min<int>(rowsToProcess, m_currentScanline);
			batch.StartScan	= m_currentScanline - batch.NumRows;
		}
		else {
			batch.NumRows	= std::min<int>(rowsToProcess, m_header->Size.Height - m_currentScanline);
			batch.StartScan	= m_currentScanline;
		}

		batch.Area = m_surface->LockSurface(
			RectInt(
			PointInt(0, batch.StartScan),
			SizeInt(m_header->Size.Width, batch.NumRows)));

		batch.Destination	= batch.Area->Buffer();
		batch.SignedScanlineStride = batch.Area->Stride();
		batch.UnsignedPixelStep = m_surface->PixelSize();

		if (m_header->FlipVertical) {
			batch.Destination += batch.SignedScanlineStride * (batch.NumRows - 1);
			batch.SignedScanlineStride = -batch.SignedScanlineStride;
		}

		return batch;
	}

	bool CodecTgaDecoder::FinalizeBatch(Batch* batch) {
		if (m_header->FlipHorizontal) {
			switch(batch->UnsignedPixelStep) {
				case 4:
				case 3:
					FlipHorizontalBatch<uint32_t>(batch);
					break;
				case 2:
					FlipHorizontalBatch<uint16_t>(batch);
					break;
				case 1:
					FlipHorizontalBatch<uint8_t>(batch);
					break;
			}
		}

		if (m_header->ColorDepth == 32 &&
			(m_header->AttributesType == TGAHeader::AT_UndefinedIgnore ||
			 m_header->AttributesType == TGAHeader::AT_NoAlpha))
		{
			// Reset the alpha bits
			int width = m_header->Size.Width;
			uint8_t* destination = batch->Destination;

			for (int y = 0; y < batch->NumRows; ++y) {
				uint8_t* currByte = destination + 3;
				for (int x = 0; x < width; ++x) {
					*currByte = 0xff;
					currByte += 4;
				}
				destination += batch->SignedScanlineStride;
			}			
		}

		m_currentScanline += batch->NumRows * (m_header->FlipVertical?-1:1);

		if ((m_currentScanline == m_header->Size.Height && m_header->FlipVertical == false) || (m_currentScanline == 0 && m_header->FlipVertical == true)) {
			return true;
		}

		return false;
	}
}
