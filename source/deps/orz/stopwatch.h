#ifndef ORZ_STOPWATCH_H
#define ORZ_STOPWATCH_H

#include <windows.h>

// TODO: Replace with boost
namespace Util {
	class StopWatch {
	public:
		void Start();
		int Stop();
		int Reset();

		StopWatch();

	private:
		enum State {
			Default,
			Stopped,
			Running,
		};

		int CurrentTime();

		int m_start, m_pause;
		State m_state;
		LARGE_INTEGER m_freq;
	};
}

#endif
