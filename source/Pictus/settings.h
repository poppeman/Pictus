#ifndef PICTUS_SETTINGS_H
#define PICTUS_SETTINGS_H

#include "appreg.h"
#include <wx/dialog.h>
#include <wx/treectrl.h>
#include <wx/button.h>

namespace App
{
	class SettingsPage;

	class Settings : public wxDialog
	{
	public:
		boost::signals2::signal<void(Reg::Settings)> OnSettingsChanged;

		void SetSettings(Reg::Settings settings);
		Settings(wxWindow* parent);

		typedef std::shared_ptr<Settings> Ptr;

	private:
		void OnOk(wxCommandEvent& evt);
		void OnApply(wxCommandEvent& evt);
		void OnCancel(wxCommandEvent& evt);

		wxTreeItemId AddSettingsPage(const std::string &name, int index, wxTreeItemId hRoot);
		void ActivatePage(int index);

		wxTreeCtrl* m_tree;
		wxSizer* m_sizer;

		wxButton* m_ok;
		wxButton* m_apply;
		wxButton* m_cancel;

		typedef std::shared_ptr<App::SettingsPage> PagePtr;
		typedef std::vector<PagePtr> PageList;

		PageList m_pages;
		Reg::Settings m_settings;

		enum {
			ButtonOkId = wxID_HIGHEST + 1,
			ButtonCancelId = wxID_HIGHEST + 2,
			ButtonApplyId = wxID_HIGHEST + 3
		};

		DECLARE_EVENT_TABLE()
	};
}

#endif
