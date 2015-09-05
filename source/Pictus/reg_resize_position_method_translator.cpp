#include "StdAfx.h"
#include "reg_resize_position_method_translator.h"

namespace Reg {
	namespace Internal {
		boost::optional<ResizePositionMethodTranslator::external_type> ResizePositionMethodTranslator::get_value(internal_type const sval) {
			if (sval == "0") return App::ResizePositionMethod::PositionToScreen;
			if (sval == "1") return App::ResizePositionMethod::PositionToCurrent;
			if (sval == "2") return App::ResizePositionMethod::PositionNothing;

			return App::IdentifierToResizePositionMethod(sval);
		}

		boost::optional<ResizePositionMethodTranslator::internal_type> ResizePositionMethodTranslator::put_value(external_type const& val) {
			return App::ResizePositionMethodToIdentifier(val);
		}
	}
}
