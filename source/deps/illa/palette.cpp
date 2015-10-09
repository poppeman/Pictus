#include "palette.h"
#include "color.h"
#include "orz/exception.h"

namespace Img
{
	void Palette::Color(int index, const Img::Color& col)
	{
		if (index > 255)
		{
			DO_THROW(Err::InvalidParam, "Color index out of bounds (> 255)");
		}

		if (index < 0)
		{
			DO_THROW(Err::InvalidParam, "Color index out of bounds (< 0)");
		}

		m_colors[index] = col;
	}

	const Img::Color& Palette::Color(int index) const
	{
		if (index > 255)
		{
			DO_THROW(Err::InvalidParam, "Color index out of bounds (> 255)");
		}

		if (index < 0)
		{
			DO_THROW(Err::InvalidParam, "Color index out of bounds (< 0)");
		}

		return m_colors[index];
	}

	Palette Grayscale(int minEntry, int maxEntry)
	{
		Palette pal;

		int entryBoundary = abs(maxEntry - minEntry);

		if (minEntry < maxEntry) {
			for (int i = 0; i <= entryBoundary; i++) {
				uint8_t intensity = static_cast<uint8_t>(i * 255 / entryBoundary);
				pal.Color(i + minEntry, Img::Color(0xff, intensity, intensity, intensity));
			}
		}
		else {
			for (int i = 0; i <= entryBoundary; i++) {
				uint8_t intensity = static_cast<uint8_t>(255 - i * 255 / entryBoundary);
				pal.Color(i + maxEntry, Img::Color(0xff, intensity, intensity, intensity));
			}
		}
		return pal;
	}

	bool operator==(const Palette& lhs, const Palette& rhs) {
		for (int i = 0; i < 256; ++i) {
			if (lhs.Color(i) != rhs.Color(i)) {
				return false;
			}
		}
		return true;
	}
}
