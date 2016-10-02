#ifndef ORZ_STOPWATCH_H
#define ORZ_STOPWATCH_H

#include <chrono>

namespace Util {
	class StopWatch {
	public:
		void Start();
		int Stop();
		int Reset();

	private:
		enum State {
			Default,
			Stopped,
			Running,
		};

		std::chrono::time_point<std::chrono::high_resolution_clock> m_start, m_pause;
		State m_state = Default;
	};
}

#endif
