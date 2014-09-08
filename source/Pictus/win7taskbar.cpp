#include "StdAfx.h"
#include "win7taskbar.h"
#include "wintypes.h"

namespace Win {
	void Win7Taskbar::AddButton(int id, int imageIndex) {
		THUMBBUTTON b;
		b.dwMask = THB_BITMAP | THB_FLAGS;
		b.dwFlags = THBF_ENABLED;
		b.iId = id;
		b.iBitmap = imageIndex;

		buttons.push_back(b);
	}

	void Win7Taskbar::Create(HWND window) {
		support = SUCCEEDED(CoCreateInstance(CLSID_TaskbarList, 0, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&itl3)));
		if(!support) return;
		hwnd = window;
		if(himler)
			itl3->ThumbBarSetImageList(window, himler);

		itl3->ThumbBarAddButtons(window, buttons.size(), &buttons.front());
	}

	Win7Taskbar::Win7Taskbar():support(false), itl3(0), himler(0) {}
	Win7Taskbar::~Win7Taskbar() {
		if(itl3)
			itl3->Release();
		itl3 = 0;
	}

	void Win7Taskbar::SetImageList(HIMAGELIST himl) {
		himler = himl;
	}
}
