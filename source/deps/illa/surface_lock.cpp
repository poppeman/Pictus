#include "surface_lock.h"

namespace Img {
	void LockStrategy::AquireLock( LockMethod method ) {
		onAcquireLock(method);
	}

	void LockStrategy::ReleaseLock( LockMethod method ) {
		onReleaseLock(method);
	}

	LockStrategy::LockStrategy() {}
	LockStrategy::~LockStrategy() {}
}
