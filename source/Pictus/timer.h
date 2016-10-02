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
	};
}

#endif
