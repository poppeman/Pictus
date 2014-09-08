#include "StdAfx.h"
#include "imagelist.h"

namespace Win {
	void ImageList::Load(DWORD id, int imageWidth, int spareImages, int transparentColor) {
		Clear();
		m_hil = ImageList_LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(id), imageWidth, spareImages, transparentColor);
	}
	void ImageList::Clear() {
		ImageList_Destroy(m_hil);
		m_hil = 0;
	}

	HIMAGELIST ImageList::Handle() {
		return m_hil;
	}

	ImageList::ImageList():m_hil(0) {
	}

	ImageList::~ImageList() {
		Clear();
	}
}
