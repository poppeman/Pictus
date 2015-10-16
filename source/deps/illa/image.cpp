#include "image.h"

namespace Img {
	using namespace Geom;

	bool Image::IsFinished() const {
		std::lock_guard<std::mutex> l(m_mxChangeState);

		return isFinished;
	}

	bool Image::IsHeaderInformationValid() const {
		std::lock_guard<std::mutex> l(m_mxChangeState);

		return m_isHeaderDataValid;
	}

	void Image::SetHeaderData(const Geom::SizeInt& size, ImageComposer::Ptr composer) {
		std::lock_guard<std::mutex> l(m_mxChangeState);
		if (composer == nullptr) {
			DO_THROW(Err::InvalidParam, "Composer was null.");
		}

		m_composer = composer;
		m_size = size;
		m_isHeaderDataValid = true;
	}

	void Image::SetMetadata(std::shared_ptr<Metadata::Document> data) {
		std::lock_guard<std::mutex> l(m_mxChangeState);
		m_metadata = data;
	}

	void Image::FinishImage(int loadTime) {
		std::lock_guard<std::mutex> l(m_mxChangeState);

		m_loadTime = loadTime;
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

	int Image::Delay() {
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

	Geom::SizeInt Image::GetSize() const {
		std::lock_guard<std::mutex> l(m_mxChangeState);
		return m_size;
	}

	Geom::SizeInt Image::GetTransformedSize() const
	{
		std::lock_guard<std::mutex> l(m_mxChangeState);
		if (m_metadata != nullptr) {
			if (m_metadata->Field[Metadata::FieldIdentifier::Orientation] != nullptr) {
				switch (m_metadata->Field[Metadata::FieldIdentifier::Orientation]->ToInteger()) {
				case 5:
				case 6:
				case 7:
				case 8:
					return{ m_size.Height, m_size.Width };
				}
			}
		}
		return m_size;
	}

	std::shared_ptr<Metadata::Document> Image::GetMetadata() const {
		std::lock_guard<std::mutex> l(m_mxChangeState);
		return m_metadata;
	}

	Image::Image() :
		m_isHeaderDataValid{ false },
		isFinished{ false },
		m_loadTime{ 0 }
	{}

	int Image::LoadTime() const {
		std::lock_guard<std::mutex> l(m_mxChangeState);

		return m_loadTime;
	}

	Surface::Ptr Image::CurrentSurface() {
		std::lock_guard<std::mutex> l(m_mxChangeState);

		if (m_isHeaderDataValid && m_composer.get()) {
			return m_composer->RequestCurrentSurface();
		}

		return Surface::Ptr();
	}

	Geom::SizeInt CalculateUnzoomedSize(std::shared_ptr<Image> img, Filter::RotationAngle angle) {
		if (img == nullptr) {
			return SizeInt(0, 0);
		}

		return Filter::CalculateUnzoomedSize(img->GetSize(), angle);
	}
}
