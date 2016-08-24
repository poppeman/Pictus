#include "settings.h"
#include <wx/sizer.h>

// Pages
//#include "settings_page.h"
#include "dlg_view.h"
/*#include "dlg_interface.h"
#include "dlg_color.h"
#include "dlg_language.h"
#include "dlg_controls.h"
#include "dlg_advanced.h"
#include "dlg_cache.h"*/
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
	END_EVENT_TABLE()

	void Settings::SetSettings(Reg::Settings settings)
	{
		m_settings = settings;
		for (auto page : m_pages)
		{
			page->UpdateFromSettings(m_settings);
		}
	}

	Settings::Settings(wxWindow *parent, Reg::Settings settings) :
		wxDialog(parent, wxID_ANY, Win::GetStringWx(SIDSettings), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER),
		m_settings(settings)
	{
		auto topSizer = new wxBoxSizer(wxVERTICAL);
		m_tree = new wxTreeCtrl(this, wxID_ANY, {0, 0}, {110, 252}, wxTR_HIDE_ROOT);
		m_sizer = new wxBoxSizer(wxHORIZONTAL);
		m_sizer->Add(m_tree, wxSizerFlags(0).Expand().Border(wxRIGHT, 10));

		m_pages.push_back(std::make_shared<SetView>(this));
		/*m_pages.push_back(std::make_shared<SetInterface>());
		m_pages.push_back(std::make_shared<SetColor>());
		m_pages.push_back(std::make_shared<SetLanguage>());
		m_pages.push_back(std::make_shared<SetControls>());
		m_pages.push_back(std::make_shared<SetKeyboard>());
		m_pages.push_back(std::make_shared<SetAdvanced>());
		m_pages.push_back(std::make_shared<SetPageCache>());*/
		m_pages.push_back(std::make_shared<SetAbout>(this));

		wxTreeItemId hPrev = 0;
		int firstIndex = -1;

		auto root = m_tree->AddRoot(L"Dummy");

		for (size_t i = 0; i < m_pages.size(); ++i)
		{
			//m_pages[i]->DoModeless(this);
			//m_pages[i]->Move(m_pagePos);
			m_pages[i]->UpdateFromSettings(m_settings);
			if (m_pages[i]->IsRootPage())
			{
				hPrev = AddSettingsPage(m_pages[i]->Caption(), static_cast<int>(i), root);
			}
			else
			{
				hPrev = AddSettingsPage(m_pages[i]->Caption(), static_cast<int>(i), hPrev);
			}

			if (firstIndex == -1)
			{
				firstIndex = static_cast<int>(i);
			}
		}

		ActivatePage(firstIndex);

		auto buttonSizer = new wxBoxSizer(wxHORIZONTAL);
		m_ok = new wxButton(this, ButtonOkId, Intl::GetString(SIDDialogOK));
		m_cancel = new wxButton(this, ButtonCancelId, Intl::GetString(SIDDialogCancel));
		m_apply = new wxButton(this, ButtonApplyId, Intl::GetString(SIDDialogApply));
		buttonSizer->Add(m_ok, wxSizerFlags(0).Right());
		buttonSizer->Add(m_cancel, wxSizerFlags(0).Right().Border(wxLEFT, 20));
		buttonSizer->Add(m_apply, wxSizerFlags(0).Right().Border(wxLEFT, 20));

		topSizer->Add(m_sizer, wxSizerFlags(1).Expand().Border(wxALL, 10));
		topSizer->Add(buttonSizer, wxSizerFlags(0).Border(wxALL, 10).Right());

		SetSizerAndFit(topSizer);

		/*Caption(SIDSettings);
		ControlText(IDOK, SIDDialogOK);
		ControlText(IDCANCEL, SIDDialogCancel);
		ControlText(IDC_APPLY, SIDDialogApply);

		CreateButton(IDOK)->OnClick.connect([this]()
											{ OnOk(); });
		CreateButton(IDC_APPLY)->OnClick.connect([this]()
												 { OnApply(); });
		CreateButton(IDCANCEL)->OnClick.connect([this]()
												{ OnCancel(); });*/
	}

/*
	HTREEITEM Settings::AddSettingsPageRoot(const std::string& name, int index, HTREEITEM hPrev) {
		return AddSettingsPageChild(name, index, TVI_ROOT, hPrev);
	}*/

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

/*
	bool Settings::UpdateTreeList() {
		HWND hView = GetDlgItem(Handle(), IDC_TREE_NAV);
		HTREEITEM hRoot = TreeView_GetRoot(hView);
		updateTreeItem(hRoot);
		return true;
	}

	void Settings::updateTreeItem(HTREEITEM hItem) {
		auto hView = GetDlgItem(Handle(), IDC_TREE_NAV);

		if (hItem) {
			TVITEM tvi;
			tvi.hItem = hItem;
			tvi.mask = TVIF_PARAM | TVIF_HANDLE;
			TreeView_GetItem(hView, &tvi);

			tvi.mask = TVIF_TEXT | TVIF_HANDLE;
			// Make a scoped copy of the string, or TreeView_SetItem will end up playing with a deleted string.
			auto wName = UTF8ToWString(m_pages[tvi.lParam]->Caption());
			tvi.pszText = const_cast<LPWSTR>(wName.c_str());
			tvi.cchTextMax = static_cast<int>(wName.length());
			TreeView_SetItem(hView, &tvi);

			updateTreeItem(TreeView_GetChild(hView, hItem));
			updateTreeItem(TreeView_GetNextSibling(hView, hItem));
		}
	}
*/
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
}
