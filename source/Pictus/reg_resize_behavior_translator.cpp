#include "StdAfx.h"
#include "reg_resize_behavior_translator.h"

namespace Reg {
	namespace Internal {
		boost::optional<ResizeBehaviorTranslator::external_type> ResizeBehaviorTranslator::get_value(internal_type const sval) {
			auto val = FromAString<int>(sval);
			if (val < 0 || val > 2) {
				return App::ResizeBehaviour::ResizeReduceOnly;
			}

			return static_cast<App::ResizeBehaviour>(val);
		}

		boost::optional<ResizeBehaviorTranslator::internal_type> ResizeBehaviorTranslator::put_value(external_type const& val) {
			return ToAString(static_cast<int>(val));
		}

	}
}
