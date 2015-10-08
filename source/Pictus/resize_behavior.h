#ifndef PICTUS_RESIZE_BEHAVIOR_H
#define PICTUS_RESIZE_BEHAVIOR_H

#pragma warning(push)
#pragma warning(disable: 4512)
#pragma warning(disable: 4503)
#include <boost/assign.hpp>
#include <boost/bimap.hpp>

namespace App {
	enum ResizeBehaviour {
		ResizeEnlargeOrReduce = 0,
		ResizeEnlargeOnly,
		ResizeReduceOnly
	};

	typedef boost::bimap<boost::bimaps::set_of<std::string>, boost::bimaps::set_of<ResizeBehaviour>> ResizeBehaviorIdentifierBimap;
	const ResizeBehaviorIdentifierBimap ResizeBehaviorIdentifier = boost::assign::list_of<ResizeBehaviorIdentifierBimap::relation>
		("EnlargeOnly", ResizeBehaviour::ResizeEnlargeOnly)
		("EnlargeOrReduce", ResizeBehaviour::ResizeEnlargeOrReduce)
		("ReduceOnly", ResizeBehaviour::ResizeReduceOnly)
		;
}

#pragma warning(pop)

#endif
