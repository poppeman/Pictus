#ifndef ILLA_SURFACE_LOCK_H
#define ILLA_SURFACE_LOCK_H

#include <memory>

namespace Img {
	enum LockMethod {
		LockRead,
		LockReadWrite
	};

	class LockStrategy {
	public:
		void AquireLock(LockMethod method);
		void ReleaseLock(LockMethod method);

		typedef std::shared_ptr<LockStrategy> Ptr;

		LockStrategy();
		virtual ~LockStrategy()=0;

	private:
		virtual	void onAcquireLock(LockMethod method)=0;
		virtual void onReleaseLock(LockMethod method)=0;
	};
}

#endif
