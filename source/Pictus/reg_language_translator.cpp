#include "StdAfx.h"
#include "reg_language_translator.h"

namespace Reg {
	namespace Internal {
		boost::optional<LanguageTranslator::external_type> LanguageTranslator::get_value(internal_type const sval) {
			auto val = FromAString<int>(sval);
			if (val == 1) return Intl::Language::Swedish;
			return Intl::Language::English;
		}

		boost::optional<LanguageTranslator::internal_type> LanguageTranslator::put_value(external_type const& val) {
			return ToAString(static_cast<int>(val));
		}

	}
}
