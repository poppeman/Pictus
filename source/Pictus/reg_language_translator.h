#ifndef PICTUS_REG_LANGUAGE_TRANSLATOR_H
#define PICTUS_REG_LANGUAGE_TRANSLATOR_H

#include <string>
#include <boost/optional.hpp>
#include <boost/property_tree/id_translator.hpp>
#include "appreg.h"

namespace Reg {
	namespace Internal {
		struct LanguageTranslator {
			typedef std::string internal_type;
			typedef Intl::Language external_type;
			boost::optional<external_type> get_value(internal_type const sval);
			boost::optional<internal_type> put_value(external_type const& val);
		};
	}
}

namespace boost{
	namespace property_tree{
		template<> struct translator_between<std::string, Intl::Language> { typedef Reg::Internal::LanguageTranslator type; };
	}
}


#endif
