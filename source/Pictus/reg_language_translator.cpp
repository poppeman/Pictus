#include "StdAfx.h"
#include "reg_language_translator.h"

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
			return nullptr;
		}

		boost::optional<LanguageTranslator::internal_type> LanguageTranslator::put_value(external_type const& val) {
			if (val == Intl::Language::English) {
				return "English";
			}
			if (val == Intl::Language::Swedish) {
				return "Swedish";
			}
			return nullptr;
		}
	}
}
