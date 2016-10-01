#ifndef ZOOMSTRATEGY_H
#define ZOOMSTRATEGY_H

#include "app_types.h"
#include "zoom_mode.h"

namespace App {
	class ZoomStrategy {
	public:
		enum {
			DefaultZoomLevel = 13,
			NumZoomLevels = 25
		};

		void ResizeBehaviour(ResizeBehaviour beh);

		void Default();

		bool ZoomIn();
		bool ZoomOut();
		void ZoomMode(App::ZoomMode mode);
		App::ZoomMode ZoomMode() const;

		struct Result {
			Geom::SizeInt ZoomedSize;
			float ZoomImage;
			float ZoomSurface;
		};

		Result CalculateViewAreaSize(const Geom::SizeInt& viewAreaSize, const Geom::SizeInt& surfaceSize, const Geom::SizeInt& imageSize);

		ZoomStrategy();
	private:
		float NonDynamicZoomLevel() const;
		//float EffectiveZoomLevel(float factor) const;
		void FindLevel();
		void ZoomFullSize();

		static const float ZoomLevels[NumZoomLevels];

		App::ZoomMode m_zoomMode;
		App::ResizeBehaviour m_resizeBehaviour = App::ResizeBehaviour::ResizeReduceOnly;
		int m_zoomLevel;
		float m_lastFitSize;
	};
}

#endif
