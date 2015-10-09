#include "timer.h"
#include "orz/exception.h"

namespace Win {
	Timer::TimerMap Timer::m_timers;

	VOID CALLBACK Timer::TimerCallback(HWND, UINT, UINT_PTR idEvent, DWORD) {
		auto i = m_timers.find(idEvent);
		if(i != m_timers.end())
			(*i->second).OnTick();
		if (KillTimer(0, idEvent) == false) {
			throw Err::CriticalError(L"Failed killing timer");
		}
	}

	void Timer::Create(_In_ _In_range_(1, 10000000) int delay) {
		Destroy();
		m_timerId = SetTimer(0, 0, delay, Timer::TimerCallback);
		if (!m_timerId) throw Err::CriticalError(L"SetTimer Failed");

		m_timers[m_timerId] = this;
	}

	void Timer::Destroy() {
		if(!m_timerId) return;
		m_timers.erase(m_timerId);
		m_timerId = 0;
	}

	Timer::Timer():m_timerId(0) {}

	Timer::~Timer() {
		Destroy();
	}
}
