#ifndef PICTUS_WIN7TASKBAR_H
#define PICTUS_WIN7TASKBAR_H

#include <windows.h>
#include <CommCtrl.h>
#include <ShObjIdl.h>

#include <vector>

namespace Win {
	class Win7Taskbar {
	public:
		void AddButton(int id, int imageIndex);

		void SetImageList(HIMAGELIST himl);
		void Create(HWND window);

		Win7Taskbar();
		~Win7Taskbar();

		Win7Taskbar(const Win7Taskbar&) = delete;
		Win7Taskbar& operator=(const Win7Taskbar&) = delete;

	private:
		std::vector<THUMBBUTTON> buttons;

		ITaskbarList3* itl3;
		bool support;
		HIMAGELIST himler;
		HWND hwnd;
	};
}

#endif
