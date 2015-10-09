#include "ctrl_statusbar.h"
#include <numeric>
#include <vector>

#include <CommCtrl.h>

namespace Win {
	void StatusBar::Add(StatusBarPart& newPart) {
		m_parts.push_back(&newPart);
		newPart.AttachTo(this);
	}

	StatusBar::StatusBar() {}

	HWND StatusBar::PerformDynamicCreate() {
		return CreateWindowEx(
			0,
			STATUSCLASSNAME,
			L"",
			WS_CHILD | WS_VISIBLE,
			50,
			50,
			50,
			50,
			DialogHandle(),
			0,
			GetModuleHandle(0),
			0);
	}

	void StatusBar::Update() {
		struct SumNonShared {
			int operator()(int lhs, StatusBarPart* rhs) {
				return lhs + (rhs->AutoSize()==StatusBarPart::AutoSizeShare?0:rhs->Width());
			}
		};
		struct IsSharedSize {
			bool operator()(StatusBarPart* v) {
				return v->AutoSize() == StatusBarPart::AutoSizeShare;
			}
		};
		int numShared = std::count_if(m_parts.begin(), m_parts.end(), IsSharedSize());
		int perShared = Position().Width() - std::accumulate(m_parts.begin(), m_parts.end(), 0, SumNonShared()) 
			/ (numShared==0?1:numShared);

		int currLeft = 0;
		std::vector<int> sizes;
		for(auto* p: m_parts) {
			int currWidth = (p->AutoSize() == StatusBarPart::AutoSizeShare?perShared:p->Width());
			sizes.push_back(currLeft + currWidth);
			currLeft += currWidth;
		}
		SendMessage(Handle(), SB_SETPARTS, sizes.size(), reinterpret_cast<LPARAM>(&sizes[0]));
		
		int i=0;
		for(auto* p: m_parts)
			SendMessage(Handle(), SB_SETTEXT, i++, reinterpret_cast<LPARAM>(p->Text().c_str()));
	}
}
