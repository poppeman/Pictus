#ifndef WALLPAPER_H
#define WALLPAPER_H

namespace Win {
	class Wallpaper:boost::noncopyable {
	public:
		enum Mode {
			WPStretch,
			WPTile,
			WPCenter
		};

		bool ApplyWallpaper(const std::wstring& file, Mode m);
	};
}

#endif
