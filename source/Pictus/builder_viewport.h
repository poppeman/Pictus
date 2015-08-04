#ifndef PICTUS_BUILDER_VIEWPORT_H
#define PICTUS_BUILDER_VIEWPORT_H

#include "viewport.h"

namespace Win {
	class BaseWindow;
}

namespace App {
	class ViewportBuilder {
	public:
		void BuildViewport(App::ViewPort& port, Win::BaseWindow* parent, Reg::Settings settings);
	};
}

#endif
