#ifndef PICTUS_REG_RESIZE_POSITION_METHOD_TRANSLATOR_H
#define PICTUS_REG_RESIZE_POSITION_METHOD_TRANSLATOR_H

#include <string>
#include "resize_position_method.h"

namespace Reg {
	namespace Internal {
		struct ResizePositionMethodTranslator {
			typedef std::string internal_type;
			typedef App::ResizePositionMethod external_type;
			boost::optional<ResizePositionMethodTranslator::external_type> get_value(internal_type const sval);
			boost::optional<ResizePositionMethodTranslator::internal_type> put_value(external_type const& val);
		};
	}
}

namespace boost{
	namespace property_tree{
		template<> struct translator_between<std::string, App::ResizePositionMethod> { typedef Reg::Internal::ResizePositionMethodTranslator type; };
	}
}

#endif
