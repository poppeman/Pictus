#ifndef PICTUS_REG_RESIZE_BEHAVIOR_TRANSLATOR_H
#define PICTUS_REG_RESIZE_BEHAVIOR_TRANSLATOR_H

#include <string>
#include <boost/optional.hpp>
#include <boost/property_tree/id_translator.hpp>
#include "appreg.h"

namespace Reg {
	namespace Internal {
		struct ResizeBehaviorTranslator {
			typedef std::string internal_type;
			typedef App::ResizeBehaviour external_type;
			boost::optional<ResizeBehaviorTranslator::external_type> get_value(internal_type const sval);
			boost::optional<ResizeBehaviorTranslator::internal_type> put_value(external_type const& val);
		};
	}
}

namespace boost{
	namespace property_tree{
		template<> struct translator_between<std::string, App::ResizeBehaviour> { typedef Reg::Internal::ResizeBehaviorTranslator type; };
	}
}

#endif
