#include "ic_imageloader.h"
#include "illa/codecmgr.h"
#include "illa/image.h"
#include "orz/exception.h"
#include "orz/fileops.h"
#include "orz/logger.h"

namespace Img {
	Geom::SizeInt ImageLoader::m_maxRes;
	std::mutex ImageLoader::m_mxHints;

	IO::OpenErrorCode ImageLoader::OpenFile() {
		return IO::OpenErrorCode::Succeeded;
	}

	LoadState ImageLoader::GetState() {
		if(m_state == ILError) return LoadStateDone;
		if(m_state == ILUnprocessed) return LoadStateUnprocessed;
		if(m_state == ILLoadedHeader) return LoadStateHeader;
		if(m_state == ILLoadingFast) return LoadStateLoading;
		if(m_state == ILDoneFast) return LoadStateHeader;
		if(m_state == ILLoadingFull) return LoadStateLoading;
		if(m_state == ILDoneFull) return LoadStateDone;
		DO_THROW(Err::CriticalError, "Unknown internal state encountered: " + ToAString(m_state));
	}

	void ImageLoader::Deallocate() {
		image->Deallocate();
		Purge();
	}

	void ImageLoader::Abort() {
		if(codec == 0) return;
		codec->Terminate();
	}

	void ImageLoader::LoadHeader() {
		try {
			if(!m_reader->Open()) {
				return FailState();
			}
			COND_STRICT(m_reader->IsOpen(), Err::FileNotOpen, "File should be open now.");

			codec = DetectAndLoadHeader(m_reader);
			if(codec == 0)
				return FailState();

			image->SetHeaderData(codec->GetSize(), codec->RequestImageComposer());
			m_state = ILLoadedHeader;
			m_sw.Stop();
		}
		catch(Err::Exception&) {
			FailState();
		}
	}

	void ImageLoader::LoadMetadata() {
		// Metadata is a nice-to-have, so don't set failstate if something goes wrong.
		try {
			if (!m_reader->IsOpen()) {
				return;
			}
			if (codec == nullptr) {
				return;
			}

			image->SetMetadata(codec->LoadMetadata());
		}
		catch (Err::Exception& e) {
			Log << L"(ImageLoader::LoadMetadata) " << UTF8ToWString(e.what()) << L"\n";
		}
	}

	void ImageLoader::Allocate() {
		COND_STRICT(codec, Err::InvalidCall, "Codec not created.");
		COND_STRICT(m_state == ILLoadedHeader || m_state == ILDoneFast, Err::InvalidCall, "Loader not in a valid state for this operation."
			"Current state: " + ToAString(m_state));

		m_sw.Reset();
		m_sw.Start();
		Geom::SizeInt resHint = GetMaximumResolutionHint();

		if(m_state == ILLoadedHeader) {
			if(!IsPositive(resHint)) {
				if(m_maySkipFast)
					goto fullalloc;
				m_state = ILDoneFast;
			}
			else switch(codec->Allocate(resHint)) {
				case AbstractCodec::AllocationStatus::NotSupported:
					if(m_maySkipFast)
						goto fullalloc;

					m_state = ILDoneFast;
					break;
				case AbstractCodec::AllocationStatus::Ok:
					m_state = ILLoadingFast;
					break;
				case AbstractCodec::AllocationStatus::Failed:
					FailState();
					break;
			}

		}
		else {
fullalloc:
			switch(codec->Allocate()) {
				case AbstractCodec::AllocationStatus::NotSupported:
					FailState();
					DO_THROW(Err::CriticalError, "Full allocation reported as unsupported.");
					break;
				case AbstractCodec::AllocationStatus::Ok:
					m_state = ILLoadingFull;
					break;
				case AbstractCodec::AllocationStatus::Failed:
					FailState();
					break;
			}
		}
		m_sw.Stop();
	}

	AbstractCodec::LoadStatus ImageLoader::LoadImageData() {
		m_sw.Start();
		const AbstractCodec::LoadStatus s = codec->LoadImageData(m_state==ILLoadingFull);
		switch(s) {
			case AbstractCodec::LoadStatus::Finished:
			case AbstractCodec::LoadStatus::Failed:
				if(m_state == ILLoadingFast)
					m_state = ILDoneFast;
				if(m_state == ILLoadingFull)
					m_state = ILDoneFull;
				image->FinishImage(m_sw.Stop());
		}
		return s;
	}

	size_t ImageLoader::MemoryRequirements() {
		if(m_state == ILError) return 0;

		if (codec == 0) DO_THROW(Err::InvalidCall, "Codec not created.");
		if (m_state < ILLoadedHeader) DO_THROW(Err::InvalidCall, "Loader not in a valid state for this operation.");
		
		return codec->EstimateMemory();
	}

	ImageLoader::ImageLoader(CodecFactoryStore* cfs, Image* img, const std::wstring& fname)
		:m_cfs(cfs), image(img), codec(0), m_maySkipFast(false), m_state(ILUnprocessed),
		m_reader(new IO::FileReader(fname)) {
		COND_STRICT(m_cfs, Err::InvalidParam, "CodecFactoryStore may not be null.");
	}
	ImageLoader::~ImageLoader() {
		Purge();
	}

	void ImageLoader::FailState() {
		Purge();
		image->FinishImage(m_sw.Stop());
		m_state = ILError;
	}

	void ImageLoader::Purge() {
		if(codec) delete codec;
		codec = 0;
		m_state = ILUnprocessed;
		m_reader->Close();
	}

	void ImageLoader::ResetAbort() {
		if(!codec) return;
		codec->ResetTerminate();
	}

	void ImageLoader::SetMaximumResolutionHint( const Geom::SizeInt& maxRes ) {
		std::lock_guard<std::mutex> l(m_mxHints);
		m_maxRes = maxRes;
	}

	Geom::SizeInt ImageLoader::GetMaximumResolutionHint() {
		std::lock_guard<std::mutex> l(m_mxHints);
		return m_maxRes;
	}

	std::wstring ImageLoader::Filename() const {
		if(!m_reader) return L"";
		return m_reader->Name();
	}

	IO::FileReader::Ptr ImageLoader::GetFileReader() {
		return m_reader;
	}

	bool ImageLoader::FastPassFinished() {
		return (m_state >= ILDoneFast);
	}

	bool ImageLoader::Error() const {
		return m_state == ILError;
	}

	Img::AbstractCodec* ImageLoader::DetectAndLoadHeader(const std::shared_ptr<IO::FileReader> reader) {
		COND_STRICT(m_cfs, Err::InvalidCall, "CodecFactoryStore was null.");
		Img::AbstractCodec* c = m_cfs->CreateCodec(IO::GetExtension(reader->Name()));
		if (c->LoadHeader(reader)) {
			return c;
		}

		const CodecFactoryStore::InfoVector& iv = m_cfs->CodecInfo();
		for (size_t i = 0; i < iv.size(); ++i) {
			c = m_cfs->CreateCodec(i);
			if (c == nullptr) {
				continue;
			}

			reader->Seek(0, IO::SeekMethod::Begin);
			if (!c->CanDetectFormat() || !c->LoadHeader(reader)) {
				delete c;
			}
			else {
				return c;
			}
		}
		return 0;
	}

	LoadEvent ImageLoader::Load() {
		try {
			switch (GetState()) {
			case LoadStateUnprocessed:
				LoadHeader();
				return LoadEventHeaderLoaded;
			case LoadStateHeader:
				LoadMetadata();
				Allocate();
				return LoadEventAllocated;
			case LoadStateLoading:
				if (GetState() == LoadStateLoading &&  LoadImageData() == AbstractCodec::LoadStatus::Finished)
					return LoadEventDone;
			}
		}
		catch (...) {
			FailState();
			throw;
		}
		return LoadEventNone;
	}

	void ImageLoader::AllowFastSkip( bool doAllow ) {
		m_maySkipFast = doAllow;
	}
}
