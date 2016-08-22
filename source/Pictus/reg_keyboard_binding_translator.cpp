#include "reg_keyboard_binding_translator.h"
#include "orz/types.h"
#include <boost/algorithm/string.hpp>

namespace Reg {
	namespace Internal {
		boost::optional<KeyboardBindingTranslator::external_type> KeyboardBindingTranslator::get_value(internal_type const sval) {
			Reg::KeyboardBinding binding;
			binding.Action = App::KeyAction::Undefined;
			binding.Key = { 0, false, false, false };

			std::vector<std::string> strs;
			boost::split(strs, sval, boost::is_any_of(","));
			if (strs.size() == 3) {
				auto keyVal = FromAString<int>(strs[0]);
				binding.Key.Key = keyVal;

				std::vector<std::string> modifiers;
				boost::split(modifiers, strs[1], boost::is_any_of("+"));

				for (auto mod : modifiers) {
					if (mod == "Alt") {
						binding.Key.Alt = true;
					}
					if (mod == "Shift") {
						binding.Key.Shift = true;
					}
					if (mod == "Ctrl") {
						binding.Key.Ctrl = true;
					}
				}

				binding.Action = App::IdentifierToKeyAction(strs[2]);
			}

			if (binding.Action == App::KeyAction::Undefined)
			{
				return boost::none;
			}

			return binding;
		}

		boost::optional<KeyboardBindingTranslator::internal_type> KeyboardBindingTranslator::put_value(external_type const& val) {
			std::vector<std::string> parts;
			parts.push_back(ToAString(val.Key.Key));
			std::vector<std::string> modifiers;
			if (val.Key.Alt) modifiers.push_back("Alt");
			if (val.Key.Shift) modifiers.push_back("Shift");
			if (val.Key.Ctrl) modifiers.push_back("Ctrl");
			parts.push_back(Implode(modifiers, "+"));
			parts.push_back(App::KeyActionToIdentifier(val.Action));

			return Implode(parts, ",");
		}
	}
}
