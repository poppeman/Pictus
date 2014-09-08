#include "StdAfx.h"

#include "surface_lock_exclusive.h"

namespace Img {
	LockStrategyExclusive::LockStrategyExclusive()
		:m_isLocked(false)
	{}

	void LockStrategyExclusive::onAcquireLock( LockMethod ) {
		std::unique_lock<std::mutex> l(m_mutexLock);
		while(m_isLocked)
			m_conditionUnlock.wait(l);

		m_isLocked = true;
	}

	void LockStrategyExclusive::onReleaseLock( LockMethod ) {
		std::lock_guard<std::mutex> l(m_mutexLock);
		m_isLocked = false;
		m_conditionUnlock.notify_all();
	}
}
