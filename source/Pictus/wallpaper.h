#ifndef PICTUS_WALLPAPER_H
#define PICTUS_WALLPAPER_H

#include <string>

namespace Win
{
	namespace Wallpaper
	{
		enum class Mode
		{
			Stretch,
			Tile,
			Center,
			FillCrop,   // Windows 7+ only
			FillPad,    // Windows 7+ only
			Span        // Windows 8+ only
		};

		bool ApplyWallpaper(const std::string& file, Mode m);
	}
}

#endif
