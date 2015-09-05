#ifndef PICTUS_RESIZE_BEHAVIOR_H
#define PICTUS_RESIZE_BEHAVIOR_H

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

#endif
