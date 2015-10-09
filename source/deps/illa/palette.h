#ifndef ILLA_PALETTE_H
#define ILLA_PALETTE_H

#include "color.h"

namespace Img {
	class Palette {
	public:
		void Color(int index, const Color& col);

		const Img::Color& Color(int index) const;

	private:
		Img::Color m_colors[256];
	};

	Img::Palette Grayscale(int minEntry = 0, int maxEntry = 255);

	bool operator==(const Palette& lhs, const Palette& rhs);
}

#endif
