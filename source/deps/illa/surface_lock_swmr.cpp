#include "surface_lock_swmr.h"
#include "orz/exception.h"
#include "orz/types.h"

namespace Img {
	LockStrategySingleWriterMultipleReaders::LockStrategySingleWriterMultipleReaders()
		:m_isWriteLocked(false)
	{}

	void LockStrategySingleWriterMultipleReaders::onAcquireLock( LockMethod method ) {
		if(method == LockReadWrite) {
			std::unique_lock<std::mutex> l(m_mutexLocks);
			while(m_isWriteLocked == true)
				m_conditionUnlock.wait(l);

			m_isWriteLocked = true;
		}
	}

	void LockStrategySingleWriterMultipleReaders::onReleaseLock( LockMethod method ) {
		if(method == LockReadWrite) {
			std::lock_guard<std::mutex> l(m_mutexLocks);

			if (m_isWriteLocked == false) DO_THROW(Err::CriticalError, L"Attempted to write-unlock a lock which wasn't write locked.");
			m_isWriteLocked = false;
			m_conditionUnlock.notify_all();
		}
	}
}
