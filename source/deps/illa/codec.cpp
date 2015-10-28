#include "codec.h"
#include "surface.h"
#include "../orz/logger.h"

namespace Img {
	bool AbstractCodec::LoadHeader(IO::FileReader::Ptr reader) {
		try {
			if (reader == nullptr) {
				DO_THROW(Err::InvalidParam, "Reader was null.");
			}
			m_file = reader;

			{
				std::lock_guard<std::mutex> l(m_state);
				m_info.SurfaceFormat = Img::Format::Undefined;
				m_info.Dimensions = Geom::SizeInt(-1, -1);
				m_fs = reader->Size();
			}

			ImageInfo ii;
			ii.SurfaceFormat = Format::Undefined;

			if (PerformLoadHeader(reader, ii)) {
				// Validate info. Invalid values are treated as codec bugs.
				if (ii.SurfaceFormat == Img::Format::Undefined) {
					DO_THROW(Err::CodecError, "Surface format was set to invalid.");
				}
				if ((ii.Dimensions.Width <= 0) || (ii.Dimensions.Height <= 0)) {
					DO_THROW(Err::CodecError, "Dimensions were set to an invalid value.");
				}

				std::lock_guard<std::mutex> l(m_state);
				m_info = ii;

				return true;
			}
		}
		catch (Err::Exception& ex) {
			m_file.reset();

			Log << "(AbstractCodec::LoadHeader) " << ex.what() << "\n";
			return false;
		}
		catch (...) {
			m_file.reset();

			Log << "(AbstractCodec::LoadHeader) Unrecognized exception.\n";
			return false;
		}

		m_file.reset();
		return false;
	}

	std::shared_ptr<Metadata::Document> AbstractCodec::LoadMetadata() {
		if (m_file == nullptr) {
			DO_THROW(Err::InvalidCall, "No file was set");
		}
		if (m_file->IsOpen() == false) {
			DO_THROW(Err::InvalidCall, "File not open");
		}
		try {
			return PerformLoadMetadata();
		}
		catch (Err::Exception& e) {
			Log << "(AbstractCodec::LoadMetadata) " << e.what() << "\n";
			return nullptr;
		}
	}

	AbstractCodec::AllocationStatus AbstractCodec::Allocate(const Geom::SizeInt& dimHint) {
		try {
			if (dimHint == Geom::SizeInt(0, 0)) {
				return PerformAllocate();
			}

			return PerformAllocate(dimHint);
		}
		catch(std::bad_alloc& e) {
			// We can't handle infinitely huge images, which is not really a fatal error.
			Log << "(AbstractCodec::Allocate) " << e.what() << "\n";
			return AllocationStatus::Failed;
		}
		catch (Err::Exception& e) {
			Log << "(AbstractCodec::Allocate) " << e.what() << "\n";
			return AllocationStatus::Failed;
		}
	}

	size_t AbstractCodec::EstimateMemory() {
		return PerformEstimateMemory();
	}

	AbstractCodec::LoadStatus AbstractCodec::LoadImageData(bool mayReset) {
		if (m_file == 0) {
			DO_THROW(Err::InvalidCall, "No file was set");
		}
		if (m_file->IsOpen() == false) {
			DO_THROW(Err::InvalidCall, "File not open");
		}

		LoadStatus l;
		try {
			l = PerformLoadImageData(m_file);
			m_isFinished = (l != LoadStatus::Aborted);
		}
		catch (Err::Exception& e) {
			Log << "(AbstractCodec::LoadImageData) " << e.what() << "\n";
			l = LoadStatus::Finished;
			m_isFinished = true;
		}

		if (m_isFinished && m_file && mayReset) {
			m_file->Close();
		}

		return l;
	}

	bool AbstractCodec::IsFinished() {
		return m_isFinished;
	}

	Geom::SizeInt AbstractCodec::GetSize() const {
		std::lock_guard<std::mutex> l(m_state);
		return m_info.Dimensions;
	}

	void AbstractCodec::Terminate() {
		m_doAbort = true;
	}

	AbstractCodec::AbstractCodec():
		m_fs{ 0 },
		m_isFinished{ false },
		m_doAbort{ false }
	{
		m_info.Dimensions = Geom::SizeInt(0, 0);
		m_info.SurfaceFormat = Img::Format::Undefined;
	}

	AbstractCodec::~AbstractCodec() {}

	bool AbstractCodec::DoTerminate() {
		return m_doAbort;
	}

	size_t AbstractCodec::PerformEstimateMemory() {
		return Area(GetSize()) * EstimatePixelSize(GetFormat());
	}

	Img::Format AbstractCodec::GetFormat() const {
		std::lock_guard<std::mutex> l(m_state);
		return m_info.SurfaceFormat;
	}

	void AbstractCodec::ResetTerminate() {
		m_doAbort = false;
	}

	std::shared_ptr<Metadata::Document> AbstractCodec::PerformLoadMetadata() {
		return nullptr;
	}

	AbstractCodec::AllocationStatus AbstractCodec::PerformAllocate() {
		AllocationStatus a = PerformAllocate(GetSize());
		if (a == AllocationStatus::NotSupported) {
			return AllocationStatus::Failed;
		}
		return a;
	}

	AbstractCodec::AllocationStatus AbstractCodec::PerformAllocate( const Geom::SizeInt& ) {
		return AllocationStatus::NotSupported;
	}

	bool AbstractCodec::CanDetectFormat() const {
		return true;
	}
}
