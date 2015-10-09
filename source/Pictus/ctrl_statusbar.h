#ifndef STATUSBAR_H
#define STATUSBAR_H

#include "ctrl_statusbar_part.h"
#include "control.h"

#include <list>

namespace Win {
	class StatusBar:public Control {
	public:
		void Add(StatusBarPart& newPart);
		void Update();

		StatusBar();
		typedef std::shared_ptr<StatusBar> Ptr;

	private:
		HWND PerformDynamicCreate();

		typedef std::list<StatusBarPart*> PartList;
		PartList m_parts;
	};
}

#endif
