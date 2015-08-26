#include "StdAfx.h"
#include "reg_resize_position_method_translator.h"

namespace Reg {
	namespace Internal {
		boost::optional<ResizePositionMethodTranslator::external_type> ResizePositionMethodTranslator::get_value(internal_type const sval) {
			auto val = FromAString<int>(sval);
			if (val < 0 || val > 2) {
				return App::ResizePositionMethod::PositionNothing;
			}

			return static_cast<App::ResizePositionMethod>(val);
		}

		boost::optional<ResizePositionMethodTranslator::internal_type> ResizePositionMethodTranslator::put_value(external_type const& val) {
			return ToAString(static_cast<int>(val));
		}
	}
}
