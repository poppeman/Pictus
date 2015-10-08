#ifndef PICTUS_REG_COLOR_TRANSLATOR_H
#define PICTUS_REG_COLOR_TRANSLATOR_H

#include <string>
#include <boost/optional.hpp>
#include <boost/property_tree/id_translator.hpp>
#include "../deps/illa/types.h"

namespace Reg {
	namespace Internal {
		struct ColorTranslator {
			typedef std::string internal_type;
			typedef Img::Color external_type;
			boost::optional<external_type> get_value(internal_type const sval);
			boost::optional<internal_type> put_value(external_type const& val);
		};
	}
}

namespace boost{
	namespace property_tree{
		template<> struct translator_between<std::string, Img::Color> { typedef Reg::Internal::ColorTranslator type; };
	}
}


#endif
