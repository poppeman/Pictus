#ifndef IMAGELOADER_H
#define IMAGELOADER_H

#include "orz/stopwatch.h"
#include "illa/codec.h"
#include "illa/codecmgr.h"

namespace Img {
	class Image;

	enum LoadState {
		LoadStateUnprocessed,
		LoadStateHeader,
		LoadStateLoading,
		LoadStateDone,
	};

	enum LoadEvent {
		LoadEventNone,
		LoadEventHeaderLoaded,
		LoadEventAllocated,
		LoadEventDone,
	};

	class ImageLoader {
	public:
		const Image* GetImage() const { return image; }
		Image* GetImage() { return image; }

		void AllowFastSkip(bool doAllow);

		LoadState GetState();
		bool Error() const;

		IO::OpenErrorCode OpenFile();

		void Deallocate();
		void Abort();
		void ResetAbort();

		LoadEvent Load();

		bool FastPassFinished();

		size_t MemoryRequirements();

		std::string Filename() const;
		IO::FileReader::Ptr GetFileReader();

		ImageLoader(CodecFactoryStore* cfs, Image* img, const std::string& fname);
		~ImageLoader();

		static void SetMaximumResolutionHint(const Geom::SizeInt& maxRes);
		static Geom::SizeInt GetMaximumResolutionHint();

		typedef std::shared_ptr<ImageLoader> Ptr;

	private:
		void Purge();
		void FailState();

		void LoadHeader();
		void LoadMetadata();
		void Allocate();
		AbstractCodec::LoadStatus LoadImageData();


		Img::AbstractCodec* DetectAndLoadHeader(const std::shared_ptr<IO::FileReader> reader);

		CodecFactoryStore* m_cfs;
		Image* image;
		AbstractCodec* codec;
		Util::StopWatch m_sw;

		enum InternalLoadState {
			ILUnprocessed,
			ILLoadedHeader,
			ILLoadingFast,
			ILDoneFast,
			ILLoadingFull,
			ILDoneFull,
			ILError,
		};

		bool m_maySkipFast;
		InternalLoadState m_state;
		IO::FileReader::Ptr m_reader;

		static std::mutex m_mxHints;
		static Geom::SizeInt m_maxRes;
	};
}

#endif
