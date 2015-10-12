#include "BasicThread.h"
#include "Logger.h"
#include "main.h"
#include "orz/exception.h"
#include "orz/types.h"

size_t BasicThread::m_idThreadCounter = 0;

void BasicThread::Terminate()
{
	Output("Terminating ... ");
	std::unique_lock<std::mutex> l(m_mutexTerm);
	m_isTerminating = true;
	l.unlock();
	m_thread->join();
}

BasicThread::BasicThread()
	:m_id(m_idThreadCounter++),
	 m_isTerminating(false)
{}

BasicThread::~BasicThread() {}

bool BasicThread::IsTerminating()
{
	std::lock_guard<std::mutex> l(m_mutexTerm);
	return m_isTerminating;
}

void BasicThread::Output(const std::string& s)
{
	OutputString(ToAString(m_id) + " - " + s);
}

void BasicThread::threadWrapper() {
	Output("Thread started, entering ThreadMain...");

	try {
		ThreadMain();
	}
	catch(Err::Exception& e) {
		SignalError("Thread: " + ToAString(m_id) + "\n" + e.what());
	}
	catch(std::exception& e) {
		SignalError("Thread: " + ToAString(m_id) + "\n" + e.what());
	}
	catch(...) {
		SignalError("Thread: " + ToAString(m_id) + "\n" + "Unknown exception type");
	}
	Output("Thread is terminating ...");
}

void BasicThread::ThreadMain()
{}

void BasicThread::Run()
{
	std::lock_guard<std::mutex> l(m_mutexTerm);

	m_isTerminating = false;
	Output("Starting thread ... ");
	m_thread = std::make_shared<std::thread>(&BasicThread::threadWrapper, this);
	Output("Thread started!");
}
