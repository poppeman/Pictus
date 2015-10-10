#include "stopwatch.h"

namespace Util {
	void StopWatch::Start() {
		if (m_state == Running) {
			return;
		}

		if (m_state == Stopped) {
			// Just adjust m_start to skip the paused period
			auto curr = std::chrono::high_resolution_clock::now();
			m_start += curr - m_pause;
		}
		else {
			m_start = std::chrono::high_resolution_clock::now();
		}

		m_state = Running;
	}

	int StopWatch::Stop() {
		if (m_state != Stopped) {
			m_pause = std::chrono::high_resolution_clock::now();

			// Temporary fix, prevents times from being negative.
			if (m_pause < m_start) {
				m_pause = m_start;
			}
		}
		m_state = Stopped;
		return std::chrono::duration_cast<std::chrono::milliseconds>(m_pause - m_start).count();
	}

	int StopWatch::Reset() {
		int time = Stop();
		m_state = Default;
		return time;
	}

	StopWatch::StopWatch():
		m_state(Default)
	{}
}
