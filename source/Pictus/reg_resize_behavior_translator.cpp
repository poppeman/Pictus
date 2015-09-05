#include "StdAfx.h"
#include "reg_resize_behavior_translator.h"

namespace Reg {
	namespace Internal {
		boost::optional<ResizeBehaviorTranslator::external_type> ResizeBehaviorTranslator::get_value(internal_type const sval) {
			if (sval == "0") return App::ResizeBehaviour::ResizeEnlargeOrReduce;
			if (sval == "1") return App::ResizeBehaviour::ResizeEnlargeOnly;
			if (sval == "2") return App::ResizeBehaviour::ResizeReduceOnly;

			return App::IdentifierToResizeBehavior(sval);
		}

		boost::optional<ResizeBehaviorTranslator::internal_type> ResizeBehaviorTranslator::put_value(external_type const& val) {
			return App::ResizeBehaviorToIdentifier(val);
		}
	}
}
