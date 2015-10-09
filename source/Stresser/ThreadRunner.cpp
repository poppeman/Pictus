#include "ThreadRunner.h"

#include "orz/orz_math.h"

#include "Logger.h"
#include <algorithm>
#include <boost/random.hpp>

struct Runner
{
	void operator()(BasicThread::Ptr p)
	{
		OutputString(L"Starting thread ...");
		p->Run();
		OutputString(L"Thread started!");
	}
};

struct Terminator
{
	void operator()(BasicThread::Ptr ptr)
	{
		ptr->Terminate();
	}
};


void ThreadRunner::ThreadMain()
{
	startThreads();

	boost::random::mt19937 rand;
	boost::random::uniform_int_distribution<> slp(0, 2000);

	while (!IsTerminating() && !m_error)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(slp(rand)));

		Output(L"Restarting");
		stopThreads();
		startThreads();
	}
	stopThreads();

	// Avoid undefined behavior due to dependent deconstruction
	m_threads.clear();
}

void ThreadRunner::startThreads()
{
	std::random_shuffle(m_threads.begin(), m_threads.end());
	std::for_each(m_threads.begin(), m_threads.end(), Runner());
	OutputString(L"All threads have been requested to start.");
}

void ThreadRunner::stopThreads()
{
	OutputString(L"Asking threads to die ...");

	std::for_each(m_threads.begin(), m_threads.end(), Terminator());
}

ThreadRunner::ThreadRunner():m_error(false)
{

}

ThreadRunner::~ThreadRunner()
{

}

void ThreadRunner::AddThread( BasicThread::Ptr threadToAdd )
{
	m_threads.push_back(threadToAdd);
}

void ThreadRunner::Stop()
{
	m_error = true;
}