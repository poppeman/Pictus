#include "StdAfx.h"

#include "filter.h"
#include "filter_int.h"
#include "filterjob.h"
#include "orz/threadpool.h"

namespace Filter {
	using namespace Geom;
	using namespace Internal;

	static const int MinimumChunkWidth = 256;

	std::shared_ptr<Util::Threadpool> pool;

	// TODO: Consider moving this into a class instead, containing the threadpool. This would remove the global threadpool and
	// allow this method to be threadsafe.
	void Scale::AutoFilter(_In_ const FilterMode mode, _In_ const FilterBuffer& source, _Inout_ FilterBuffer& dest, _In_ const Geom::RectInt& uncroppedRegion, _In_ Img::Format format, _In_ _In_range_(0, 1000000000) float zoom) {
		// TODO: Make this function thread-safe. Function assumes that only one thread calls this function, otherwise the thread pool gets messed up.
		if (mode < 0 || mode >= FilterNum) {
			DO_THROW(Err::InvalidParam, TX("Invalid filter mode."));
		}

		if (source.BufferData == nullptr || dest.BufferData == nullptr) {
			DO_THROW(Err::InvalidParam, TX("Null buffer not allowed."));
		}

		if ((uncroppedRegion.Top() < 0) || (uncroppedRegion.Left() < 0)) {
			DO_THROW(Err::InvalidParam, TX("Negative coordinates in region."));
		}

		auto region = CropResampleRegions(uncroppedRegion, source.Dimensions, dest.Dimensions, zoom);

		if (region.Dimensions().Width == 0 || region.Dimensions().Height == 0) {
			// Empty region, do nothing.
			return;
		}

		if (IsPositive(region.Dimensions()) == false) {
			DO_THROW(Err::InvalidParam, TX("Invalid region."));
		}

		// We can't let this initialize statically, as that will mess up DLL support.
		if (!pool) {
			pool = std::make_shared<Util::Threadpool>(std::max<unsigned int>(2, std::thread::hardware_concurrency()));
		}

		FilterJob::SetSource(mode, &source, format, zoom);

		if (zoom == 1.0f)
			return DirectCopy(source, dest, region, format);

		// TODO: Make this prettier. 
		if(region.Width() > MinimumChunkWidth && region.Height() > FilterJob::ChunkSize) {
			std::vector<std::shared_ptr<FilterJob>> jobs;

			int num_segments = region.Height() / FilterJob::ChunkSize + 1;
			int num_prior = jobs.size();
			int num_diff = num_segments - num_prior;
			jobs.resize(num_segments);
			for(int i = 0; i < num_diff; ++i)
				jobs[i+num_prior] = std::make_shared<FilterJob>();

			for(int i = 0; i < num_segments; ++i) {
				jobs[i]->Construct(i, dest, region);
				pool->AddTask([&jobs, i]() { jobs[i]->Run(); });
			}
			pool->JoinAll();
			return;
		}

		FilterJob f;
		f.Construct(-1, dest, region);
		f.Run();
	}
}
