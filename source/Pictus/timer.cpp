#include "timer.h"
#include "orz/exception.h"

namespace Win {
	void Timer::Notify() {
		OnTick();
	}

	void Timer::Create(int delay) {
		delay = std::max(1, delay);  // Things pretty much hang if delay=0
		if (!Start(delay))
		{
			DO_THROW(Err::CriticalError, "SetTimer Failed");
		}
	}

	void Timer::Destroy() {
		Stop();
	}

	Timer::Timer() {}

	Timer::~Timer() {
		Destroy();
	}
}
