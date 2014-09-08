#ifndef CODEC_H
#define CODEC_H

#include "orz/file.h"

#include "types.h"
#include "surface.h"
#include "imagecomposer.h"

namespace Img {
	class AbstractCodec {
	public:
		enum class LoadStatus {
			Finished,
			Aborted,
			Failed,
		};

		enum class AllocationStatus {
			Ok,
			Failed,
			NotSupported,
		};

		virtual ImageComposer::Ptr RequestImageComposer()=0;
		bool LoadHeader(IO::FileReader::Ptr reader);
		AllocationStatus Allocate(const Geom::SizeInt& dimHint = Geom::SizeInt(0, 0));
		size_t EstimateMemory();
		LoadStatus LoadImageData(bool mayReset = true);
		Geom::SizeInt GetSize() const;
		Img::Format GetFormat() const;
		void Terminate();
		void ResetTerminate();
		bool CanDetectFormat() const;

		AbstractCodec();
		virtual ~AbstractCodec();

		AbstractCodec(const AbstractCodec&) = delete;
		AbstractCodec& operator=(const AbstractCodec&) = delete;

		typedef std::shared_ptr<AbstractCodec> Ptr;

	protected:
		bool IsFinished();
		bool DoTerminate();

		struct ImageInfo {
			Geom::SizeInt Dimensions;
			Img::Format SurfaceFormat;
		};

	private:
		virtual bool PerformLoadHeader(IO::FileReader::Ptr file, ImageInfo& info)=0;
		virtual AllocationStatus PerformAllocate();
		virtual AllocationStatus PerformAllocate(const Geom::SizeInt& dimHint);
		virtual LoadStatus PerformLoadImageData(IO::FileReader::Ptr file)=0;
		virtual size_t PerformEstimateMemory();

		mutable std::mutex m_state;

		IO::FileReader::Ptr m_file;
		ImageInfo m_info;
		FileInt m_fs;
		std::atomic<bool> m_isFinished;
		std::atomic<bool> m_doAbort;
	};
}

#endif
