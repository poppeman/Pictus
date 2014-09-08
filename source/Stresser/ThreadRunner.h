
#ifndef ThreadRunner_h__
#define ThreadRunner_h__

#include <deque>
#include "BasicThread.h"

class ThreadRunner
	:public BasicThread
{
public:
	void							AddThread(BasicThread::Ptr threadToAdd);

	void							ThreadMain();

	void Stop();

	ThreadRunner();
	~ThreadRunner();

	typedef std::shared_ptr<ThreadRunner> Ptr;

private:
	void							stopThreads();
	void							startThreads();

	bool m_error;
	std::deque<BasicThread::Ptr>	m_threads;
};

#endif // ThreadRunner_h__
