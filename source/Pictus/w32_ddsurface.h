#ifndef PICTUS_W32_DDSURFACE_H
#define PICTUS_W32_DDSURFACE_H

#include "illa/types.h"

#include <memory>

namespace Win {
	class DDSurface {
	public:
		typedef std::shared_ptr<DDSurface> Ptr;
		void Create(const Geom::SizeInt& dims);

		Geom::SizeInt Dimensions();

	private:
		virtual void OnCreate(const Geom::SizeInt& dims)=0;

		Geom::SizeInt m_dims;
	};
}

#endif
