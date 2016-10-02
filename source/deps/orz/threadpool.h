#ifndef ORZ_THREADPOOL_H
#define ORZ_THREADPOOL_H

#include <boost/function.hpp>
#include <boost/thread.hpp>

#include <condition_variable>
#include <list>
#include <mutex>

namespace Util {
	class Threadpool {
	public:
		typedef boost::function<void()> Task;
		void AddTask(Task func);
		void JoinAll();

		Threadpool(size_t numThreads);
		~Threadpool();

		Threadpool(const Threadpool&) = delete;
		Threadpool& operator=(const Threadpool&) = delete;

	private:
		void Worker();

		std::mutex m_mxTasks;
		std::condition_variable m_cdWorkAvail;
		std::condition_variable m_cdIdle;
		std::list<Task> m_tasks;
		boost::thread_group m_threads;
		bool m_terminate = false;
		size_t m_numRunning = 0;
	};
}

#endif
