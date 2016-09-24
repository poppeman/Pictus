#ifndef TIMER_H
#define TIMER_H

#include <wx/timer.h>

#include <boost/signals2.hpp>

namespace Win {
	class Timer:private wxTimer {
	public:
		boost::signals2::signal<void()> OnTick;

		void Create(int delay);
		void Destroy();

		Timer();
		~Timer();

		Timer(const Timer&) = delete;
		Timer& operator=(const Timer&) = delete;

	private:
		void Notify() override;
		//static VOID CALLBACK TimerCallback(HWND, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);

		//UINT_PTR m_timerId;

		//typedef std::map<UINT_PTR, Timer*> TimerMap;
		//static TimerMap m_timers;
	};
}

#endif
