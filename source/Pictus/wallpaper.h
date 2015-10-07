#ifndef WALLPAPER_H
#define WALLPAPER_H

namespace Win {
	class Wallpaper:boost::noncopyable {
	public:
		enum class Mode {
			Stretch,
			Tile,
			Center,
			FillCrop,   // Windows 7+ only
			FillPad,    // Windows 7+ only
			Span        // Windows 8+ only
		};

		bool ApplyWallpaper(const std::wstring& file, Mode m);
	};
}

#endif
