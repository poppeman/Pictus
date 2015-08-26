#include "StdAfx.h"
#include "reg_mouse_action_translator.h"

namespace Reg {
	namespace Internal {
		boost::optional<MouseActionTranslator::external_type> MouseActionTranslator::get_value(internal_type const sval) {
			auto val = FromAString<int>(sval);

			if (val < 0 || val > static_cast<int>(App::MouseAction::MouseUndefined)) {
				return App::MouseAction::MouseDisable;
			}

			return static_cast<App::MouseAction>(val);
		}

		boost::optional<MouseActionTranslator::internal_type> MouseActionTranslator::put_value(external_type const& val) {
			return ToAString(static_cast<int>(val));
		}
	}
}
