#include "threadpool.h"

namespace Util {
	void Threadpool::AddTask(Task func) {
		std::lock_guard<std::mutex> l(m_mxTasks);
		m_tasks.push_back(func);
		m_cdWorkAvail.notify_all();
	}

	void Threadpool::JoinAll() {
		std::unique_lock<std::mutex> l(m_mxTasks);
		while (m_numRunning || !m_tasks.empty()) {
			m_cdIdle.wait(l);
		}
	}

	Threadpool::Threadpool(size_t numThreads)
	{
		std::lock_guard<std::mutex> l(m_mxTasks);
		for (size_t i = 0; i < numThreads; i++) {
			// False positive. thread_group will clean up all threads automatically on destruction
			// coverity[RESOURCE_LEAK]
			m_threads.create_thread([this]() { Worker(); });
		}
	}

	Threadpool::~Threadpool() {
		JoinAll();
		{
			std::lock_guard<std::mutex> l(m_mxTasks);
			m_terminate = true;
			m_cdWorkAvail.notify_all();
		}
		m_threads.join_all();
	}

	void Threadpool::Worker() {
		std::unique_lock<std::mutex> l(m_mxTasks);
		while(!m_terminate) {
			m_cdIdle.notify_all();
			while (m_tasks.empty() && !m_terminate) {
				m_cdWorkAvail.wait(l);
			}

			if (m_terminate) {
				return;
			}

			m_numRunning++;
			auto t = m_tasks.front();
			m_tasks.pop_front();
			l.unlock();
			t();
			l.lock();
			m_numRunning--;
		}
	}
}
