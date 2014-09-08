#ifndef STRESSTEST_BASICTHREAD_H
#define STRESSTEST_BASICTHREAD_H

class BasicThread {
public:
	void Run();
	void Terminate();

	BasicThread();
	virtual ~BasicThread();

	typedef std::shared_ptr<BasicThread> Ptr;

protected:
	bool IsTerminating();
	void Output(const std::wstring& s);

private:
	void threadWrapper();
	virtual void ThreadMain();

	static size_t m_idThreadCounter;
	std::mutex m_mutexTerm;
	size_t m_id;
	bool m_isTerminating;
	std::shared_ptr<std::thread> m_thread;
};

#endif
