#include "StdAfx.h"

#include "BasicThread.h"
#include "Logger.h"
#include "main.h"

size_t BasicThread::m_idThreadCounter = 0;

void BasicThread::Terminate()
{
	Output(TX("Terminating ... "));
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

void BasicThread::Output(const std::wstring& s)
{
	OutputString(ToWString(m_id) + TX(" - ") + s);
}

void BasicThread::threadWrapper() {
	Output(TX("Thread started, entering ThreadMain..."));

	try {
		ThreadMain();
	}
	catch(Err::Exception& e) {
		SignalError(TX("Thread: ") + ToWString(m_id) + TX("\n") + e.Desc());
	}
	catch(std::exception& e) {
		SignalError(TX("Thread: ") + ToWString(m_id) + TX("\n") + ToWString(e.what()));
	}
	catch(...) {
		SignalError(TX("Thread: ") + ToWString(m_id) + TX("\n") + TX("Unknown exception type"));
	}
	Output(TX("Thread is terminating ..."));
}

void BasicThread::ThreadMain()
{}

void BasicThread::Run()
{
	std::lock_guard<std::mutex> l(m_mutexTerm);

	m_isTerminating = false;
	Output(TX("Starting thread ... "));
	m_thread = std::make_shared<std::thread>(&BasicThread::threadWrapper, this);
	Output(TX("Thread started!"));
}
