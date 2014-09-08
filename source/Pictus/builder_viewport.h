
#include "viewport.h"

namespace Win {
	class BaseWindow;
}

namespace App {
	class ViewportBuilder {
	public:
		void BuildViewport(App::ViewPort& port, Win::BaseWindow* parent);
	};
}

