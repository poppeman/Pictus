#ifndef PICTUS_REG_KEYBOARD_BINDING_TRANSLATOR_H
#define PICTUS_REG_KEYBOARD_BINDING_TRANSLATOR_H

#include <string>
#include "appreg.h"

namespace Reg {
	namespace Internal {
		struct KeyboardBindingTranslator {
			typedef std::string internal_type;
			typedef Reg::KeyboardBinding external_type;
			boost::optional<external_type> get_value(internal_type const sval);
			boost::optional<internal_type> put_value(external_type const& val);
		};
	}
}

namespace boost{
	namespace property_tree{
		template<> struct translator_between<std::string, Reg::KeyboardBinding> { typedef Reg::Internal::KeyboardBindingTranslator type; };
	}
}


#endif
