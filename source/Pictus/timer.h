#ifndef TIMER_H
#define TIMER_H

namespace Win {
	class Timer:boost::noncopyable {
	public:
		boost::signals2::signal<void()> OnTick;

		void Create(_In_ _In_range_(1, 10000000) int delay);
		void Destroy();

		Timer();
		~Timer();

	private:
		static VOID CALLBACK TimerCallback(HWND, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);

		UINT_PTR m_timerId;

		typedef std::map<UINT_PTR, Timer*> TimerMap;
		static TimerMap m_timers;
	};
}

#endif
