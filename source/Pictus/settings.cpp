#include "settings.h"
#include "dlg_view.h"
#include "dlg_interface.h"
#include "dlg_color.h"
#include "dlg_language.h"
#include "dlg_controls.h"
#include "dlg_advanced.h"
#include "dlg_cache.h"
#include "dlg_about.h"
#include "dlg_keyboard.h"
#include "wintypes.h"

#include "settings_layout.h"

#include <wx/sizer.h>

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

	Settings::Settings(wxWindow *parent, Reg::Settings& settings) :
		wxDialog(parent, wxID_ANY, Win::GetStringWx(SIDSettings), wxDefaultPosition, { 720, 650 }, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER | wxSTAY_ON_TOP),
		m_settings(settings)
	{
		auto topSizer = new wxBoxSizer(wxVERTICAL);
		m_tree = new wxTreeCtrl(this, wxID_ANY, {0, 0}, {150, 252}, wxTR_HIDE_ROOT);
		m_tree->Bind(wxEVT_TREE_SEL_CHANGED, &Settings::OnTreeItemSelected, this);
		m_sizer = new wxBoxSizer(wxHORIZONTAL);
		m_sizer->Add(m_tree, wxSizerFlags(0).Expand().Border(wxRIGHT, 10));

		m_pages.push_back(std::make_shared<SetView>(this));
		m_pages.push_back(std::make_shared<SetInterface>(this));
		m_pages.push_back(std::make_shared<SetColor>(this));
		m_pages.push_back(std::make_shared<SetLanguage>(this));
		m_pages.push_back(std::make_shared<SetControls>(this));
		m_pages.push_back(std::make_shared<SetKeyboard>(this));
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

		ActivatePage(std::max(0, firstIndex));

		auto buttonSizer = new wxBoxSizer(wxHORIZONTAL);
		m_ok = new wxButton(this, wxID_ANY, Intl::GetString(SIDDialogOK));
		m_ok->Bind(wxEVT_BUTTON, &Settings::OnOk, this);
		m_cancel = new wxButton(this, wxID_CANCEL, Intl::GetString(SIDDialogCancel));
		m_cancel->Bind(wxEVT_BUTTON, &Settings::OnCancel, this);
		m_apply = new wxButton(this, wxID_ANY, Intl::GetString(SIDDialogApply));
		m_apply->Bind(wxEVT_BUTTON, &Settings::OnApply, this);
		buttonSizer->Add(m_ok, wxSizerFlags(0));
		buttonSizer->Add(m_cancel, ButtonPadding());
		buttonSizer->Add(m_apply, ButtonPadding());

		topSizer->Add(m_sizer, wxSizerFlags(1).Expand().Border(wxALL, 10));
		topSizer->Add(buttonSizer, wxSizerFlags(0).Border(wxALL, 10).Right());

		SetSizer(topSizer);
	}


	wxTreeItemId Settings::AddSettingsPage(const std::string &name, int index, wxTreeItemId hRoot)
	{
		return m_tree->AppendItem(hRoot, wxString::FromUTF8(name.c_str()), -1, -1, new TreeItemData(index));
	}

	void Settings::ActivatePage(int index)
	{
		// OnTreeItemSelected might trigger during destruction, after m_pages have been destroyed.
		if (m_pages.empty())
		{
			return;
		}

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
