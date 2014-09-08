#ifndef WIN7TASKBAR_H
#define WIN7TASKBAR_H

namespace Win {
	class Win7Taskbar:boost::noncopyable {
	public:
		void AddButton(int id, int imageIndex);

		void SetImageList(HIMAGELIST himl);
		void Create(HWND window);

		Win7Taskbar();
		~Win7Taskbar();

	private:
		std::vector<THUMBBUTTON> buttons;

		ITaskbarList3* itl3;
		bool support;
		HIMAGELIST himler;
		HWND hwnd;
	};
}

#endif
