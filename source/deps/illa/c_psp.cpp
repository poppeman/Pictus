#include "c_psp.h"
#include "orz/bitconvert.h"
#include "orz/logger.h"
#include "psp_chunkskipper.h"
#include "surface_locked_area.h"

const uint8_t Img::CodecPSP::FileHeader[32]={0x50, 0x61, 0x69, 0x6E, 0x74, 0x20, 0x53, 0x68, 0x6F, 0x70, 0x20, 0x50, 0x72, 0x6F, 0x20, 0x49, 0x6D, 0x61, 0x67, 0x65, 0x20, 0x46, 0x69, 0x6C, 0x65, 0x0A, 0x1A, 0x00, 0x00, 0x00, 0x00};
const uint8_t Img::CodecPSP::Gen_HeaderID[4]={0x7E, 0x42, 0x4B, 0x00};

namespace Img {
	using namespace Geom;
	using namespace Internal;

	bool CodecPSP::PerformLoadHeader(IO::FileReader::Ptr file, ImageInfo& info) {
		uint8_t header[32];
		if (file->Read(header, 1, 32) != 32) {
			Log << "(CodecPSP::PerformLoadHeader) Failed loading magic bytes\n";
			return false;
		}

		for (int i = 0; i < 32; ++i) {
			if (header[i] != FileHeader[i]) {
				Log << "(CodecPSP::PerformLoadHeader) Magic bytes mismatch\n";
				return false;
			}
		}

		uint16_t ver;
		file->ReadFull(&ver, 2);

		if (ver < PSPVersion5) {
			Log << "(CodecPSP::PerformLoadHeader) File version too old, file is not supported.\n";
			return false;
		}

		// Skip minor version
		file->Seek(2, IO::SeekMethod::Current);

		// Read general image attributes
		uint8_t headid[4];
		if (file->Read(headid, 1, 4) != 4) {
			Log << "(CodecPSP::PerformLoadHeader) Failed loading general image attributes header.\n";
			return false;
		}

		for (int i = 0; i < 4; ++i) {
			if (headid[i] != Gen_HeaderID[i]) {
				Log << "(CodecPSP::PerformLoadHeader) General image attributes header mismatch.\n";
				return false;
			}
		}

		// Check block ID
		uint16_t blockid;
		file->ReadFull(&blockid, 2);
		if (blockid != PSP_IMAGE_BLOCK) {
			return false;
		}

		uint32_t blen;
		file->ReadFull(&blen, 4);

		// Load image attributes
		file->Seek(4, IO::SeekMethod::Current);
		long w, h;
		file->ReadFull(&w, 4);
		file->ReadFull(&h, 4);

		// Skip weird shit
		file->Seek(8, IO::SeekMethod::Current);
		uint8_t resmetric;
		file->ReadFull(&resmetric, 1);

		m_compression = PSP_COMP_NONE;
		file->ReadFull(&m_compression, 2);
		if (m_compression != PSP_COMP_NONE && m_compression != PSP_COMP_RLE) {
			return false;
		}

		m_bitDepth = 0;
		uint16_t tmpBitdepth;
		file->ReadFull(&tmpBitdepth, 2);
		m_bitDepth = static_cast<uint8_t>(tmpBitdepth);

		if (m_bitDepth == 1 || m_bitDepth == 4 || m_bitDepth == 8) {
			info.SurfaceFormat = Img::Format::Index8;
		}
		else if (m_bitDepth == 24) {
			info.SurfaceFormat = Img::Format::XRGB8888;
		}
		else {
			return false;
		}

		// Seek back to the first block
		file->Seek(36, IO::SeekMethod::Begin);

		info.Dimensions = Geom::SizeInt(w, h);

		return true;
	}

	AbstractCodec::AllocationStatus CodecPSP::PerformAllocate() {
		GetSurface()->CreateSurface(GetSize(), GetFormat());
		GetSurface()->ClearSurface(Img::Color(0xff, 0, 0, 0));
		return AllocationStatus::Ok;
	}

	AbstractCodec::LoadStatus CodecPSP::PerformLoadImageData(IO::FileReader::Ptr file) {
		if (m_compression != PSP_COMP_NONE && m_compression != PSP_COMP_RLE) {
			Log << "(CodecPSP::PerformLoadImageData) Compression method " << ToAString(m_compression) << " is not supported.\n";
			return LoadStatus::Failed;
		}

		ParseFileStructure(file);

		while (DoTerminate() == false) {
			if (m_currScanline == GetSurface()->Height()) {
				return LoadStatus::Finished;
			}

			int scansToProcess = std::min<int>(GetSurface()->Height() - m_currScanline, ScansPerChunk);
			auto area  = GetSurface()->LockSurface(RectInt(PointInt(0, m_currScanline), SizeInt(GetSurface()->Width(), scansToProcess)));
			uint8_t* currChunkStart = area->Buffer();

			if (DecodeScanlines(file, scansToProcess, GetSurface()->Width(), area->Stride(), GetSurface()->PixelSize(), currChunkStart) == false) {
				area->Unlock();
				Log << "(CodecPSP::PerformLoadImageData) Scanline decode failed.\n";
				return LoadStatus::Failed;
			}

			m_currScanline += scansToProcess;
			area->Unlock();
		}
		return LoadStatus::Aborted;
	}

	CodecPSP::CodecPSP():
		m_isStructureParsed{ false },
		m_currScanline{ 0 }
	{}

	std::wstring readVariableLengthString(IO::FileReader::Ptr file) {
		uint16_t length;
		file->ReadFull(&length, 2);
		std::wstring str;
		str.resize(length);

		for(int i = 0; i < length; ++i) {
			uint8_t c;
			file->ReadFull(&c, 1);
			str[i] = c;
		}
		return str;
	}

	Geom::RectInt readRect(IO::FileReader::Ptr file) {
		uint32_t coord[4];
		file->ReadFull(coord, 16);
		return { 
			{ static_cast<int>(coord[0]), static_cast<int>(coord[1]) },
			Geom::SizeInt{ static_cast<int>(coord[2] - coord[0]), static_cast<int>(coord[3] - coord[1]) }
		};
	}

	bool CodecPSP::ReadBlockHeader( IO::FileReader::Ptr reader, BlockHeader* outHeader ) {
		uint8_t header_id[4];

		outHeader->PositionStart = reader->Position();

		reader->ReadFull(&header_id, 4);
		outHeader->ID = PSP_IMAGE_BLOCK;
		reader->ReadFull(&outHeader->ID, 2);
		outHeader->Length = 0;
		reader->ReadFull(&outHeader->Length, 4);

		outHeader->PositionEnd = outHeader->PositionStart + outHeader->Length + 10;

		for (int i = 0; i < 4; ++i) {
			if (header_id[i] != Gen_HeaderID[i]) {
				Log << "(CodecPSP::ReadBlockHeader) Block header signature mismatch.\n";
				return false;
			}
		}

		return true;
	}

	bool CodecPSP::ReadLayerBlock(IO::FileReader::Ptr reader, Layer* outLayer) {
		std::wstring layername(readVariableLengthString(reader));

		// Fetch layer type (PSPLayerType)
		outLayer->Type = LTUndefined;
		reader->ReadFull(&outLayer->Type, 1);

		readRect(reader); // silly data
		outLayer->SavedLayerArea = readRect(reader);

		reader->Seek(1, IO::SeekMethod::Current);
		outLayer->BlendMode = LAYER_BLEND_NORMAL;
		reader->ReadFull(&outLayer->BlendMode, 1);
		reader->ReadFull(&outLayer->LayerFlags, 1);

		return true;
	}

	bool CodecPSP::ReadChannelBlock(IO::FileReader::Ptr reader, Channel* outChannel) {
		outChannel->CurrentSourcePosition = 0;

		reader->ReadFull(&outChannel->SizeCompressed, 4);
		reader->ReadFull(&outChannel->SizeUncompressed, 4);
		outChannel->TypeDIB = PSP_DIB_IMAGE;
		reader->ReadFull(&outChannel->TypeDIB, 2);
		outChannel->TypeChannel = PSP_CHANNEL_COMPOSITE;
		reader->ReadFull(&outChannel->TypeChannel, 2);

		return true;
	}

	void CodecPSP::ParseFileStructure(IO::FileReader::Ptr reader) {
		if (m_isStructureParsed == true) {
			return;
		}

		m_isStructureParsed = true;

		try {
			for (;;) {
				BlockHeader currentBlockHeader;
				if (ReadBlockHeader(reader, &currentBlockHeader) == false) {
					return;
				}

				bool skip = true;

				switch(currentBlockHeader.ID) {
					case PSP_LAYER_START_BLOCK:
						skip = false;
						break;

					case PSP_TABLE_BANK_BLOCK:
						skip = false;
						{
							ChunkSkipper c(reader);

							uint16_t numBitmaps = 0;
							reader->ReadFull(&numBitmaps, sizeof(numBitmaps));
							if (numBitmaps != 1)
								return;
						}
						break;

					case PSP_COLOR_BLOCK:
						{
							uint32_t numColors = 0;
							{
								ChunkSkipper c(reader);
								reader->ReadFull(&numColors, 4);
								if (numColors > 256)
									return;

							}

							Img::Palette pal;
							for (uint32_t i = 0; i < numColors; ++i) {
								uint32_t rgbQuad;
								reader->ReadFull(&rgbQuad, 4);
								rgbQuad |= 0xff000000;
								pal.Color(i, Color::FromDWord(rgbQuad));
							}
							GetSurface()->SetPalette(pal);
						}
						break;

					case PSP_LAYER_BLOCK:
						skip = false;
						{
							ChunkSkipper c(reader);
							ReadLayerBlock(reader, &m_layer);
						}
						{
							ChunkSkipper c(reader);
							uint16_t numBitmaps;
							reader->ReadFull(&numBitmaps, 2);
							uint16_t numChannels;
							reader->ReadFull(&numChannels, 2);
						}
						break;

					case PSP_CHANNEL_BLOCK:
						{
							Channel currentChannel;

							{
								ChunkSkipper c(reader);
								ReadChannelBlock(reader, &currentChannel);
							}

							currentChannel.PositionData = reader->Position();
							m_channels.push_back(currentChannel);
						}
						break;
				}

				// Skip the rest of the block
				if (skip) {
					reader->Seek(currentBlockHeader.PositionEnd, IO::SeekMethod::Begin);
				}
			}
		}
		catch (Err::EndOfFile&) {
			return;
		}
	}

	bool CodecPSP::DecodeScanlines(IO::FileReader::Ptr reader, int scansToProcess, int width, size_t stride, size_t pixelSize, uint8_t* destination) {
		if (m_compression == PSP_COMP_NONE) {
			return DecodeRawScanlines(reader, scansToProcess, width, stride, pixelSize, destination);
		}
		else if (m_compression == PSP_COMP_RLE) {
			return DecodeRLEScanlines(reader, scansToProcess, width, stride, pixelSize, destination);
		}

		return false;
	}

	bool CodecPSP::DecodeRawScanlines(IO::FileReader::Ptr reader, int scansToProcess, int width, size_t stride, size_t pixelSize, uint8_t* destination) {
		if(m_channels.size() == 1 && (m_bitDepth == 4 || m_bitDepth == 1)) {
			Channel& currChannel = m_channels.at(0);

			// Scanlines are padded on a four-byte boundary
			size_t bytesPerScanline = ((width + 7) / 8) * m_bitDepth;

			auto inputBuffer(std::make_unique<uint8_t[]>(bytesPerScanline));
			reader->Seek(currChannel.PositionData + currChannel.CurrentSourcePosition, IO::SeekMethod::Begin);

			for (int y = 0; y < scansToProcess; ++y) {
				reader->ReadFull(inputBuffer.get(), bytesPerScanline);

				Util::ConvertToBytes(destination, inputBuffer.get(), m_bitDepth, width);

				currChannel.CurrentSourcePosition += bytesPerScanline;
				destination += stride;
			}
		}
		else if (m_channels.size() == 1 && m_bitDepth == 8) {
			Channel& currChannel = m_channels.at(0);

			reader->Seek(currChannel.PositionData + currChannel.CurrentSourcePosition, IO::SeekMethod::Begin);

			for (int y = 0; y < scansToProcess; ++y) {
				reader->ReadFull(destination, width);
				destination += stride;
			}
			currChannel.CurrentSourcePosition += scansToProcess * width;

		}
		else if (m_channels.size() == 3 && m_bitDepth == 24) {
			auto inputBuffer(std::make_unique<uint8_t[]>(width));

			for (int channel = 0; channel < 3; ++channel) {
				Channel& currChannel = m_channels.at(2 - channel);
				reader->Seek(currChannel.PositionData + currChannel.CurrentSourcePosition, IO::SeekMethod::Begin);

				uint8_t* currScanStart = destination + channel;

				for (int y = 0; y < scansToProcess; ++y) {
					uint8_t* currPixel = currScanStart;

					reader->ReadFull(inputBuffer.get(), width);

					for (int x = 0; x < width; ++x) {
						*currPixel = *(inputBuffer.get() + x);
						currPixel += pixelSize;
					}
					currScanStart += stride;
				}

				currChannel.CurrentSourcePosition += scansToProcess * width;
			}
		}
		else {
			Log << "(CodecPSP::DecodeRawScanlines) Pixel format not supported.\n";
			return false;
		}

		return true;
	}

	bool CodecPSP::DecodeRLEScanlines( IO::FileReader::Ptr reader, int scansToProcess, int width, size_t stride, size_t pixelSize, uint8_t* destination ) {
		if (m_channels.size() == 1 && (m_bitDepth == 4 || m_bitDepth == 1)) {
			Channel& currChannel = m_channels.at(0);

			// Scanlines are padded on a four-byte boundary
			int bytesPerScanline = ((width + 7) / 8) * m_bitDepth;

			auto inputBuffer(std::make_unique<uint8_t[]>(bytesPerScanline));
			reader->Seek(currChannel.PositionData + currChannel.CurrentSourcePosition, IO::SeekMethod::Begin);
			m_fileStream.SetFileReader(reader);

			for (int y = 0; y < scansToProcess; ++y) {
				DecodeRLEChannel(reader, 1, bytesPerScanline, stride, 1, inputBuffer.get());
				Util::ConvertToBytes(destination, inputBuffer.get(), m_bitDepth, width);
				destination += stride;
			}

			currChannel.CurrentSourcePosition = m_fileStream.VirtualFilePosition() - currChannel.PositionData;
		}
		else if (m_channels.size() == 1 && m_bitDepth == 8) {
			Channel& currChannel = m_channels.at(0);

			reader->Seek(currChannel.PositionData + currChannel.CurrentSourcePosition, IO::SeekMethod::Begin);
			m_fileStream.SetFileReader(reader);

			if (DecodeRLEChannel(reader, scansToProcess, width, stride, pixelSize, destination) == false) {
				Log << "(CodecPSP::DecodeRLEScanlines) Failed decoding channel.\n";
				return false;
			}

			currChannel.CurrentSourcePosition = m_fileStream.VirtualFilePosition() - currChannel.PositionData;
		}
		else if (m_channels.size() == 3 && m_bitDepth == 24) {
			for (size_t channel = 0; channel < 3; ++channel) {
				Channel& currChannel = m_channels.at(2 - channel);
				reader->Seek(currChannel.PositionData + currChannel.CurrentSourcePosition, IO::SeekMethod::Begin);
				m_fileStream.SetFileReader(reader);
				uint8_t* currScanStart = destination + channel;
				if (DecodeRLEChannel(reader, scansToProcess, width, stride, pixelSize, currScanStart) == false) {
					Log << "(CodecPSP::DecodeRLEScanlines) Failed decoding channel.\n";
					return false;
				}

				currChannel.CurrentSourcePosition = m_fileStream.VirtualFilePosition() - currChannel.PositionData;
			}
		}
		else {
			Log << "(CodecPSP::DecodeRLEScanlines) Pixel format not supported.\n";
			return false;
		}

		return true;
	}

	bool CodecPSP::DecodeRLEChannel( IO::FileReader::Ptr reader, int scansToProcess, int width, size_t stride, size_t pixelSize, uint8_t* destination ) {
		for (int y = 0; y < scansToProcess; ++y) {
			int x = 0;
			while (x < width) {
				uint8_t count = m_fileStream.ReadByte();
				if (count > 128) {
					count -= 128;
					if (count + x > width) {
						Log << "(CodecPSP::DecodeRLEChannel) RLE block out of bounds.\n";
						return false;
					}

					uint8_t data = m_fileStream.ReadByte();
					for (int pixel = 0; pixel < count; ++pixel) {
						*(destination + (x + pixel) * pixelSize) = data;
					}

					x += count;
				}
				else {
					if (count + x > width) {
						Log << "(CodecPSP::DecodeRLEChannel) RAW block out of bounds.\n";
						return false;
					}

					for (int pixel = 0; pixel < count; ++pixel) {
						*(destination + (x + pixel) * pixelSize) = m_fileStream.ReadByte();
					}

					x += count;
				}
			}
			destination += stride;
		}
		return true;
	}
}
