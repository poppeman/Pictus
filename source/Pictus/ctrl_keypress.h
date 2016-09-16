#ifndef PICTUS_APP_CTRL_KEYPRESS_H
#define PICTUS_APP_CTRL_KEYPRESS_H

#include "app_types.h"

#include <wx/panel.h>
#include <wx/textctrl.h>

namespace App
{
	class Keypress:public wxPanel
	{
	public:
		std::function<void(App::KeyboardPress)> OnNewCombo;
		void SetCombo(App::KeyboardPress kp);

		Keypress(wxWindow* parent);

	private:
		wxTextCtrl* m_edit;
	};
}

#endif
