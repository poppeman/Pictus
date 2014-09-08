#ifndef SURFACE_LOCK_EXCLUSIVE_H
#define SURFACE_LOCK_EXCLUSIVE_H

#include "surface_lock.h"

namespace Img {
	class LockStrategyExclusive:public LockStrategy {
	public:
		LockStrategyExclusive();

	private:
		void onAcquireLock(LockMethod method);
		void onReleaseLock(LockMethod method);

		std::mutex m_mutexLock;
		std::condition_variable m_conditionUnlock;

		bool m_isLocked;
	};
}

#endif
