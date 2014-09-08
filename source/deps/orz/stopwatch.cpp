#include "StdAfx.h"
#include "stopwatch.h"

namespace Util {
	void StopWatch::Start() {
		if (m_state == Running) {
			return;
		}

		if (m_state == Stopped) {
			// Just adjust m_start to skip the paused period
			int curr = CurrentTime();
			m_start += curr - m_pause;
		}
		else {
			m_start = CurrentTime();
		}

		m_state = Running;
	}

	int StopWatch::Stop() {
		if (m_state != Stopped) {
			m_pause = CurrentTime();

			// Temporary fix, prevents times from being negative.
			if (m_pause < m_start) {
				m_pause = m_start;
			}
		}
		m_state = Stopped;
		return m_pause - m_start;
	}

	int StopWatch::Reset() {
		int time = Stop();
		m_state = Default;
		return time;
	}

	StopWatch::StopWatch() :m_state(Default) {
		QueryPerformanceFrequency(&m_freq);
	}

	int StopWatch::CurrentTime() {
		LARGE_INTEGER curr;
		QueryPerformanceCounter(&curr);

		return static_cast<int>((1000 * (curr.QuadPart)) / m_freq.QuadPart);
	}
}
