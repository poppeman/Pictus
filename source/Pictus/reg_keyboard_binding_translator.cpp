#include "StdAfx.h"
#include "reg_keyboard_binding_translator.h"

namespace Reg {
	namespace Internal {
		boost::optional<KeyboardBindingTranslator::external_type> KeyboardBindingTranslator::get_value(internal_type const sval) {
			Reg::KeyboardBinding binding;
			binding.Action = App::KeyAction::Undefined;
			binding.Key = 0;

			std::vector<std::string> strs;
			boost::split(strs, sval, boost::is_any_of(","));
			if (strs.size() == 3) {
				auto keyVal = FromAString<int>(strs[0]);
				binding.Key = keyVal;
				binding.Action = App::IdentifierToKeyAction(strs[2]);
			}

			return binding;
		}

		boost::optional<KeyboardBindingTranslator::internal_type> KeyboardBindingTranslator::put_value(external_type const& val) {
			std::vector<std::string> parts;
			parts.push_back(ToAString(val.Key));
			parts.push_back("?");
			parts.push_back(App::KeyActionToIdentifier(val.Action));

			return Implode(parts, ",");
		}
	}
}
