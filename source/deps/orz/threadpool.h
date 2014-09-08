#ifndef ORZ_THREADPOOL_H
#define ORZ_THREADPOOL_H

namespace Util {
	class Threadpool:boost::noncopyable {
	public:
		typedef boost::function<void()> Task;
		void AddTask(Task func);
		void JoinAll();

		Threadpool(size_t numThreads);
		~Threadpool();

	private:
		void Worker();

		std::mutex m_mxTasks;
		std::condition_variable m_cdWorkAvail;
		std::condition_variable m_cdIdle;
		std::list<Task> m_tasks;
		boost::thread_group m_threads;
		bool m_terminate;
		size_t m_numRunning;
	};
}

#endif
