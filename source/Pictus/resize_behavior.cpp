#include "StdAfx.h"
#include "resize_behavior.h"
#include <boost/assign.hpp>
#include <boost/bimap.hpp>

namespace App {
	typedef boost::bimap<boost::bimaps::set_of<std::string>, boost::bimaps::set_of<ResizeBehaviour>> ResizeBehaviorIdentifierBimap;
	const ResizeBehaviorIdentifierBimap cg_resizeBehaviorIdentifiers = boost::assign::list_of<ResizeBehaviorIdentifierBimap::relation>
		("EnlargeOnly", ResizeBehaviour::ResizeEnlargeOnly)
		("EnlargeOrReduce", ResizeBehaviour::ResizeEnlargeOrReduce)
		("ReduceOnly", ResizeBehaviour::ResizeReduceOnly)
		;


	std::string ResizeBehaviorToIdentifier(ResizeBehaviour action) {
		return cg_resizeBehaviorIdentifiers.right.find(action)->second;
	}

	App::ResizeBehaviour IdentifierToResizeBehavior(std::string identifier) {
		return cg_resizeBehaviorIdentifiers.left.find(identifier)->second;
	}
}
