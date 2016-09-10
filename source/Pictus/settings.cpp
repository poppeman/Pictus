#include "settings.h"
#include <wx/sizer.h>

// Pages
//#include "settings_page.h"
#include "dlg_view.h"
#include "dlg_interface.h"
#include "dlg_color.h"
#include "dlg_language.h"
#include "dlg_controls.h"
#include "dlg_advanced.h"
#include "dlg_cache.h"
#include "dlg_about.h"
#include "wintypes.h"
/*#include "dlg_keyboard.h"

#include "ctrl_button.h"

#include <Uxtheme.h>*/

namespace App
{
	using namespace Intl;

	class TreeItemData : public wxTreeItemData
	{
	public:
		int Index;

		TreeItemData(int index) :
			Index(index)
		{}
	};

	BEGIN_EVENT_TABLE (Settings, wxDialog)
		EVT_BUTTON(ButtonOkId, Settings::OnOk)
		EVT_BUTTON(ButtonCancelId, Settings::OnCancel)
		EVT_BUTTON(ButtonApplyId, Settings::OnApply)
		EVT_TREE_SEL_CHANGED(TreeCtrlId, Settings::OnTreeItemSelected)
	END_EVENT_TABLE()

	Settings::Settings(wxWindow *parent, Reg::Settings& settings) :
		wxDialog(parent, wxID_ANY, Win::GetStringWx(SIDSettings), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER),
		m_settings(settings)
	{
		auto topSizer = new wxBoxSizer(wxVERTICAL);
		m_tree = new wxTreeCtrl(this, TreeCtrlId, {0, 0}, {150, 252}, wxTR_HIDE_ROOT);
		m_sizer = new wxBoxSizer(wxHORIZONTAL);
		m_sizer->Add(m_tree, wxSizerFlags(0).Expand().Border(wxRIGHT, 10));

		m_pages.push_back(std::make_shared<SetView>(this));
		m_pages.push_back(std::make_shared<SetInterface>(this));
		m_pages.push_back(std::make_shared<SetColor>(this));
		m_pages.push_back(std::make_shared<SetLanguage>(this));
		m_pages.push_back(std::make_shared<SetControls>(this));
		//m_pages.push_back(std::make_shared<SetKeyboard>());
		m_pages.push_back(std::make_shared<SetAdvanced>(this));
		m_pages.push_back(std::make_shared<SetPageCache>(this));
		m_pages.push_back(std::make_shared<SetAbout>(this));

		wxTreeItemId hPrev = 0;
		int firstIndex = -1;

		auto root = m_tree->AddRoot(L"Dummy");

		for (size_t i = 0; i < m_pages.size(); ++i)
		{
			m_pages[i]->UpdateFromSettings(m_settings);
			if (m_pages[i]->IsRootPage())
			{
				hPrev = AddSettingsPage(m_pages[i]->Caption(), static_cast<int>(i), root);
			}
			else
			{
				AddSettingsPage(m_pages[i]->Caption(), static_cast<int>(i), hPrev);
			}

			if (firstIndex == -1)
			{
				firstIndex = static_cast<int>(i);
			}
		}
		m_tree->ExpandAll();

		ActivatePage(firstIndex);

		auto buttonSizer = new wxBoxSizer(wxHORIZONTAL);
		m_ok = new wxButton(this, ButtonOkId, Intl::GetString(SIDDialogOK));
		m_cancel = new wxButton(this, ButtonCancelId, Intl::GetString(SIDDialogCancel));
		m_apply = new wxButton(this, ButtonApplyId, Intl::GetString(SIDDialogApply));
		buttonSizer->Add(m_ok, wxSizerFlags(0));
		buttonSizer->Add(m_cancel, wxSizerFlags(0).Border(wxLEFT, 20));
		buttonSizer->Add(m_apply, wxSizerFlags(0).Border(wxLEFT, 20));

		topSizer->Add(m_sizer, wxSizerFlags(1).Expand().Border(wxALL, 10));
		topSizer->Add(buttonSizer, wxSizerFlags(0).Border(wxALL, 10).Right());

		SetSizerAndFit(topSizer);
	}


	wxTreeItemId Settings::AddSettingsPage(const std::string &name, int index, wxTreeItemId hRoot)
	{
		return m_tree->AppendItem(hRoot, wxString::FromUTF8(name.c_str()), -1, -1, new TreeItemData(index));
	}

	void Settings::ActivatePage(int index)
	{
		auto count = static_cast<int>(m_sizer->GetItemCount());
		if (count == 2)
		{
			auto lastItem = count - 1;
			auto oldWin = m_sizer->GetItem(lastItem)->GetWindow();
			oldWin->Hide();
			m_sizer->Detach(oldWin);
		}
		m_sizer->Add(m_pages[index].get(), wxSizerFlags(1).Expand());
		m_pages[index]->Show(true);
		m_sizer->Layout();
	}

	void Settings::OnOk(wxCommandEvent &evt)
	{
		OnApply(evt);
		Hide();
	}

	void Settings::OnApply(wxCommandEvent& evt)
	{
		// Store all pages data and update strings
		for (size_t i = 0; i < m_pages.size(); ++i)
		{
			m_pages[i]->WriteSettings(m_settings);
		}

		//UpdateTreeList();
		OnSettingsChanged(m_settings);
	}

	void Settings::OnCancel(wxCommandEvent& evt)
	{
		Hide();
	}

	void Settings::OnTreeItemSelected(wxTreeEvent &evt)
	{
		auto itemData = dynamic_cast<TreeItemData*>(m_tree->GetItemData(evt.GetItem()));
		if(itemData != nullptr)
		{
			ActivatePage(itemData->Index);
		}
	}
}
