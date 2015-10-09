#include "BasicThread.h"
#include "Logger.h"
#include "main.h"
#include "orz/exception.h"
#include "orz/types.h"

size_t BasicThread::m_idThreadCounter = 0;

void BasicThread::Terminate()
{
	Output(L"Terminating ... ");
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
	OutputString(ToWString(m_id) + L" - " + s);
}

void BasicThread::threadWrapper() {
	Output(L"Thread started, entering ThreadMain...");

	try {
		ThreadMain();
	}
	catch(Err::Exception& e) {
		SignalError(L"Thread: " + ToWString(m_id) + L"\n" + UTF8ToWString(e.what()));
	}
	catch(std::exception& e) {
		SignalError(L"Thread: " + ToWString(m_id) + L"\n" + UTF8ToWString(e.what()));
	}
	catch(...) {
		SignalError(L"Thread: " + ToWString(m_id) + L"\n" + L"Unknown exception type");
	}
	Output(L"Thread is terminating ...");
}

void BasicThread::ThreadMain()
{}

void BasicThread::Run()
{
	std::lock_guard<std::mutex> l(m_mutexTerm);

	m_isTerminating = false;
	Output(L"Starting thread ... ");
	m_thread = std::make_shared<std::thread>(&BasicThread::threadWrapper, this);
	Output(L"Thread started!");
}
