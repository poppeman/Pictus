#include "StdAfx.h"
#include "image.h"

namespace Img {
	using namespace Geom;

	_Use_decl_annotations_ bool Image::IsFinished() const {
		std::lock_guard<std::mutex> l(m_mxChangeState);

		return isFinished;
	}

	_Use_decl_annotations_ bool Image::IsHeaderInformationValid() const {
		std::lock_guard<std::mutex> l(m_mxChangeState);

		return m_isHeaderDataValid;
	}

	_Use_decl_annotations_ void Image::SetHeaderData(const Geom::SizeInt& size, ImageComposer::Ptr composer) {
		std::lock_guard<std::mutex> l(m_mxChangeState);
		if (composer == nullptr) {
			DO_THROW(Err::InvalidParam, TX("Composer was null."));
		}

		m_composer = composer;
		m_size = size;
		m_isHeaderDataValid = true;
	}

	_Use_decl_annotations_ void Image::FinishImage(int loadTime) {
		std::lock_guard<std::mutex> l(m_mxChangeState);

		loadTime = loadTime;
		isFinished = true;
	}

	void Image::Deallocate() {
		std::lock_guard<std::mutex> l(m_mxChangeState);
		isFinished = false;
		m_composer.reset();
	}

	void Image::NextFrame() {
		std::lock_guard<std::mutex> l(m_mxChangeState);

		if (m_isHeaderDataValid) {
			m_composer->Advance();
		}
	}

	_Use_decl_annotations_ int Image::Delay() {
		std::lock_guard<std::mutex> l(m_mxChangeState);

		if (m_isHeaderDataValid && m_composer.get()) {
			return m_composer->Delay();
		}

		return -1;
	}

	void Image::RestartAnimation() {
		std::lock_guard<std::mutex> l(m_mxChangeState);

		if (m_isHeaderDataValid && m_composer) {
			m_composer->Restart();
		}
	}

	_Use_decl_annotations_ Geom::SizeInt Image::GetSize() const {
		std::lock_guard<std::mutex> l(m_mxChangeState);
		return m_size;
	}

	Image::Image(std::wstring filename):
		m_isHeaderDataValid{ false },
		isFinished{ false },
		m_loadTime{ 0 }
	{}

	_Use_decl_annotations_ int Image::LoadTime() const {
		std::lock_guard<std::mutex> l(m_mxChangeState);

		return m_loadTime;
	}

	_Use_decl_annotations_ Surface::Ptr Image::CurrentSurface() {
		std::lock_guard<std::mutex> l(m_mxChangeState);

		if (m_isHeaderDataValid && m_composer.get()) {
			return m_composer->RequestCurrentSurface();
		}

		return Surface::Ptr();
	}
}
