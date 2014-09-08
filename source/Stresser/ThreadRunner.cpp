#include "StdAfx.h"

#include "ThreadRunner.h"

#include "orz/orz_math.h"

#include "Logger.h"

struct Runner
{
	void operator()(BasicThread::Ptr p)
	{
		OutputString(TX("Starting thread ..."));
		p->Run();
		OutputString(TX("Thread started!"));
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

	Math::Randomizer rand;

	while (!IsTerminating() && !m_error)
	{
		Sleep(rand.Random() % 2000);

		Output(TX("Restarting"));
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
	OutputString(TX("All threads have been requested to start."));
}

void ThreadRunner::stopThreads()
{
	OutputString(TX("Asking threads to die ..."));

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