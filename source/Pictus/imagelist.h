#ifndef IMAGELIST_H
#define IMAGELIST_H

namespace Win {
	class ImageList {
	public:
		void Load(DWORD id, int imageWidth, int spareImages, int transparentColor);
		void Clear();

		HIMAGELIST Handle();

		ImageList();
		~ImageList();

	private:
		HIMAGELIST m_hil;
	};
}

#endif
