#include "zoomstrategy.h"
#include "illa/geom.h"
#include "orz/sysinfo.h"

namespace App {
	using namespace Geom;

	const float ZoomStrategy::ZoomLevels[NumZoomLevels]	= {
		0.01f, 0.02f, 0.03f, 0.05f, 0.07f, 0.10f, 0.125f, 0.15f, 0.20f, 0.25f, 0.30f, 0.5f, 0.7f, // Min
		1.0f, // Default
		1.5f, 2.0f, 3.0f, 5.0f, 7.0f, 10.0f, 20.0f, 30.0f, 50.0f, 70.0f, 100.0f};


	bool ZoomStrategy::ZoomIn() {
		if (ZoomMode() == App::ZoomFullSize) {
			m_zoomLevel = DefaultZoomLevel;
		}

		if (ZoomMode() == ZoomFitImage) {
			FindLevel();
		}

		m_zoomMode = ZoomFree;

		if (m_zoomLevel < (NumZoomLevels - 1)) {
			m_zoomLevel++;
			return true;
		}
		return false;
	}

	ZoomStrategy::ZoomStrategy():m_lastFitSize(1.0f), m_zoomMode(ZoomFree), m_zoomLevel(DefaultZoomLevel) {}

	bool ZoomStrategy::ZoomOut() {
		if (ZoomMode() == App::ZoomFullSize) {
			m_zoomLevel = DefaultZoomLevel;
		}

		if (ZoomMode() == ZoomFitImage) {
			FindLevel();
		}

		m_zoomMode = ZoomFree;

		if (m_zoomLevel > 0) {
			m_zoomLevel--;
			return true;
		}
		return false;
	}

	void ZoomStrategy::ZoomMode(App::ZoomMode mode) {
		switch(mode) {
			case App::ZoomFree:
			case App::ZoomFitImage:
			case App::ZoomFullSize:
				break;

			default:
				DO_THROW(Err::InvalidParam, TX("Invalid zooming mode. Nasty stuff."));
		}
		m_zoomMode = mode;
	}

	App::ZoomMode ZoomStrategy::ZoomMode() const {
		return m_zoomMode;
	}

	float ZoomStrategy::NonDynamicZoomLevel() const {
		switch(ZoomMode()) {
			case App::ZoomFullSize:
				return 1.0;

			case App::ZoomFree:
				return ZoomLevels[m_zoomLevel];

			default:
				DO_THROW(Err::CriticalError, TX("Invalid zoom mode set."));
		}
	}

	void ZoomStrategy::FindLevel() {
		m_zoomLevel = 0;

		for (int i = 1; i < NumZoomLevels; i++) {
			if (m_lastFitSize >= ZoomLevels[i])
				m_zoomLevel = i;
			else
				return;
		}
		m_zoomLevel	= NumZoomLevels - 1;
	}

	void ZoomStrategy::ResizeBehaviour(App::ResizeBehaviour beh) {
		m_resizeBehaviour = beh;
	}

	ZoomStrategy::Result ZoomStrategy::CalculateViewAreaSize(const Geom::SizeInt& viewAreaSize, const Geom::SizeInt& surfaceSize, const Geom::SizeInt& imageSize) {
		Result r = {SizeInt(0, 0), 0.0f, 0.0f};

		if(!IsPositive(imageSize)) 
			return r;

		if (m_zoomMode == ZoomFitImage) {
			Geom::SizeFloat scales = viewAreaSize.StaticCast<float>() / imageSize.StaticCast<float>();
			float fScale = std::min(scales.Width, scales.Height);

			// Adjust scale to acceptable values
			switch(m_resizeBehaviour) {
				case ResizeEnlargeOnly:
					fScale = std::max(fScale, 1.0f);
					break;
				case ResizeReduceOnly:
					fScale = std::min(fScale, 1.0f);
					break;
			}

			r.ZoomImage = fScale;
			m_lastFitSize = fScale;
		}
		else {
			r.ZoomImage = NonDynamicZoomLevel();
		}

		float factor = surfaceSize.Width?static_cast<float>(imageSize.Width) / surfaceSize.Width:0.0f;
		r.ZoomSurface = r.ZoomImage * factor;

		SizeInt zoomed = RoundCast(surfaceSize * r.ZoomSurface);
		r.ZoomedSize = Minimum(zoomed, viewAreaSize);
		return r;
	}
}
