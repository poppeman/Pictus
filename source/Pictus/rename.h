#ifndef PICTUS_DIALOGS_RENAME_H
#define PICTUS_DIALOGS_RENAME_H

#include <wx/dialog.h>
#include <wx/textctrl.h>

namespace App
{
	class Rename:public wxDialog
	{
	public:
		const std::string& Name();

		Rename(wxWindow* parent, const std::string& currname);

	private:
		wxTextCtrl* m_filename;

	private:
		void OnOk();
		void OnCancel();

		std::string m_name;
	};
}

#endif
