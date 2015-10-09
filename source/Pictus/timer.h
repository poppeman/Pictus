#ifndef TIMER_H
#define TIMER_H

#pragma warning(push)
#pragma warning(disable: 4512)
#include <boost/signals2.hpp>

#include <windows.h>

namespace Win {
	class Timer {
	public:
		boost::signals2::signal<void()> OnTick;

		void Create(_In_ _In_range_(1, 10000000) int delay);
		void Destroy();

		Timer();
		~Timer();

		Timer(const Timer&) = delete;
		Timer& operator=(const Timer&) = delete;

	private:
		static VOID CALLBACK TimerCallback(HWND, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);

		UINT_PTR m_timerId;

		typedef std::map<UINT_PTR, Timer*> TimerMap;
		static TimerMap m_timers;
	};
}

#pragma warning(pop)

#endif
