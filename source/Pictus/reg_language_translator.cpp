#include "reg_language_translator.h"
#include "orz/types.h"

namespace Reg {
	namespace Internal {
		// TODO: Centralize (de)serialization the same way as for keyboard events

		boost::optional<LanguageTranslator::external_type> LanguageTranslator::get_value(internal_type const sval) {
			if (sval == "English") {
				return Intl::Language::English;
			}
			if (sval == "Swedish") {
				return Intl::Language::Swedish;
			}
			auto val = FromAString<int>(sval);
			if (val == 0) return Intl::Language::English;
			if (val == 1) return Intl::Language::Swedish;
			return boost::none;
		}

		boost::optional<LanguageTranslator::internal_type> LanguageTranslator::put_value(external_type const& val) {
			if (val == Intl::Language::English) {
				return std::string("English");
			}
			if (val == Intl::Language::Swedish) {
				return std::string("Swedish");
			}
			return boost::none;
		}
	}
}
