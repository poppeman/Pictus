#ifndef PICTUS_REG_ZOOM_MODE_TRANSLATOR_H
#define PICTUS_REG_ZOOM_MODE_TRANSLATOR_H

#include <string>
#include <boost/optional.hpp>
#include <boost/property_tree/id_translator.hpp>
#include "zoom_mode.h"

namespace Reg {
	namespace Internal {
		struct ZoomModeTranslator {
			typedef std::string internal_type;
			typedef App::ZoomMode external_type;
			boost::optional<external_type> get_value(internal_type const sval);
			boost::optional<internal_type> put_value(external_type const& val);
		};
	}
}

namespace boost{
	namespace property_tree{
		template<> struct translator_between<std::string, App::ZoomMode> { typedef Reg::Internal::ZoomModeTranslator type; };
	}
}


#endif
