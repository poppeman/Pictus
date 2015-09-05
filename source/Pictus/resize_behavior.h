#ifndef PICTUS_RESIZE_BEHAVIOR_H
#define PICTUS_RESIZE_BEHAVIOR_H

namespace App {
	enum ResizeBehaviour {
		ResizeEnlargeOrReduce = 0,
		ResizeEnlargeOnly,
		ResizeReduceOnly
	};

	std::string ResizeBehaviorToIdentifier(ResizeBehaviour action);
	ResizeBehaviour IdentifierToResizeBehavior(std::string identifier);
}

#endif
