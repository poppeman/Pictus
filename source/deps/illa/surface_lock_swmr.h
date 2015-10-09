#ifndef SURFACE_LOCK_SWMR_H
#define SURFACE_LOCK_SWMR_H

#include "surface_lock.h"

#include <condition_variable>
#include <mutex>

namespace Img {
	class LockStrategySingleWriterMultipleReaders:public LockStrategy {
	public:
		LockStrategySingleWriterMultipleReaders();

	private:
		void onAcquireLock(LockMethod method);
		void onReleaseLock(LockMethod method);

		std::mutex m_mutexLocks;
		std::condition_variable m_conditionUnlock;

		bool m_isWriteLocked;
	};
}

#endif
