#ifndef PICTUS_BUILDER_VIEWPORT_H
#define PICTUS_BUILDER_VIEWPORT_H

#include "viewport.h"

namespace App {
	class ViewportBuilder {
	public:
		void BuildViewport(App::ViewPort& port, wxWindow* parent, const Reg::Settings& settings);
	};
}

#endif
