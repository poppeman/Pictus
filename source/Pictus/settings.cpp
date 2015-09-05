#include "StdAfx.h"
#include "settings.h"
#include "res_settings.h"

// Pages
#include "settings_page.h"
#include "dlg_view.h"
#include "dlg_interface.h"
#include "dlg_color.h"
#include "dlg_language.h"
#include "dlg_controls.h"
#include "dlg_advanced.h"
#include "dlg_cache.h"
#include "dlg_about.h"
#include "dlg_keyboard.h"

#include "ctrl_button.h"

namespace App {
	using namespace Intl;
	using Geom::PointInt;

	void Settings::PerformOnCreate() {
		Caption(SIDSettings);
		ControlText(IDOK, SIDDialogOK);
		ControlText(IDCANCEL, SIDDialogCancel);
		ControlText(IDC_APPLY, SIDDialogApply);

		CreateButton(IDOK)->OnClick.connect([this]() { OnOk(); });
		CreateButton(IDC_APPLY)->OnClick.connect([this]() { OnApply(); });
		CreateButton(IDCANCEL)->OnClick.connect([this]() { OnCancel(); });
	}


	bool Settings::PerformOnInitDialog() {
		RECT rect;
		GetWindowRect(GetDlgItem(Handle(), IDC_TREE_NAV), &rect);
		POINT pt = {rect.right, rect.top};

		ScreenToClient(Handle(), &pt);

		auto m_pagePos = PointInt(pt.x, pt.y);

		// Create treeview
		HTREEITEM hPrev  = 0;
		HTREEITEM hRoot  = 0;
		HTREEITEM hFirst = 0;

		for (auto i = 0; i < m_pages.size(); ++i) {
			m_pages[i]->DoModeless(this);
			m_pages[i]->Move(m_pagePos);
			m_pages[i]->UpdateFromSettings(m_settings);
			if (m_pages[i]->IsRootPage()) {
				if (hRoot != 0)
					hPrev = hRoot;

				hPrev = AddSettingsPageRoot(m_pages[i]->Caption(), static_cast<int>(i), hPrev);
				hRoot = 0;
			}
			else {
				if (hRoot == 0) {
					hRoot = hPrev;
					hPrev = 0;
				}

				hPrev = AddSettingsPageChild(m_pages[i]->Caption(), static_cast<int>(i), hRoot, hPrev);
			}

			if (hFirst == 0)
				hFirst = hPrev;
		}

		// Pick first page by default
		TreeView_SelectItem(GetDlgItem(Handle(), IDC_TREE_NAV), hFirst);

		SetWindowTheme(GetDlgItem(Handle(), IDC_TREE_NAV), L"Explorer", 0);

		return true;
	}

	bool Settings::PerformOnNotify(DWORD id, LPNMHDR pnmh) {
		switch(id) {
			case IDC_TREE_NAV:
				{
					LPNMTREEVIEW pnmtv = (LPNMTREEVIEW)pnmh;
					switch(pnmh->code) {
						// Windows tends to send the wrong message, so look for either version
						case TVN_SELCHANGEDW:
						case TVN_SELCHANGEDA:
							m_pages[pnmtv->itemOld.lParam]->Show(false);
							m_pages[pnmtv->itemNew.lParam]->Show(true);
							break;
					}
					break;
				}
			default:
				return false;
		}
		return true;
	}

	void Settings::SetSettings(Reg::Settings settings) {
		m_settings = settings;
		for (auto page : m_pages) {
			page->UpdateFromSettings(m_settings);
		}
	}

	void Settings::FreeResources() {
		TreeView_DeleteAllItems(GetDlgItem(Handle(), IDC_TREE_NAV));
	}

	Settings::Settings():
		Win::Dialog(IDD_SETTINGS)
	{
		m_pages.push_back(std::make_shared<SetView>());
		m_pages.push_back(std::make_shared<SetInterface>());
		m_pages.push_back(std::make_shared<SetColor>());
		m_pages.push_back(std::make_shared<SetLanguage>());
		m_pages.push_back(std::make_shared<SetControls>());
		m_pages.push_back(std::make_shared<SetKeyboard>());
		m_pages.push_back(std::make_shared<SetAdvanced>());
		m_pages.push_back(std::make_shared<SetPageCache>());
		m_pages.push_back(std::make_shared<SetAbout>());
	}

	HTREEITEM Settings::AddSettingsPageRoot(const std::wstring& name, int index, HTREEITEM hPrev) {
		return AddSettingsPageChild(name, index, TVI_ROOT, hPrev);
	}

	HTREEITEM Settings::AddSettingsPageChild(const std::wstring& name, int index, HTREEITEM hRoot, HTREEITEM hPrev) {
		TVINSERTSTRUCT	tvins;

		SetTextTVITEM(&tvins.item, name);
		tvins.item.mask = TVIF_TEXT | TVIF_PARAM;
		tvins.item.lParam = index;
		tvins.hInsertAfter = hPrev;
		tvins.hParent	= hRoot;

		HWND hView = GetDlgItem(Handle(), IDC_TREE_NAV);
		HTREEITEM hItem = reinterpret_cast<HTREEITEM>(SendMessage(hView, TVM_INSERTITEM, 0, (LPARAM)&tvins));
		TreeView_SetItemState(hView, hItem, TVIS_EXPANDED, TVIS_EXPANDED);
		return hItem;
	}

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
			std::wstring name = m_pages[tvi.lParam]->Caption();
			SetTextTVITEM(&tvi, name);
			TreeView_SetItem(hView, &tvi);

			updateTreeItem(TreeView_GetChild(hView, hItem));
			updateTreeItem(TreeView_GetNextSibling(hView, hItem));
		}
	}

	void Settings::SetTextTVITEM(TVITEM* tvi, const std::wstring &name) {
		tvi->pszText = const_cast<LPWSTR>(name.c_str());
		tvi->cchTextMax	= static_cast<int>(name.length());
	}

	void Settings::OnOk() {
		OnApply();
		FreeResources();
		DestroyWindow(Handle());
	}

	void Settings::OnApply() {
		// Store all pages data and update strings
		for (size_t i = 0; i < m_pages.size(); ++i) {
			m_pages[i]->WriteSettings(m_settings);
		}

		UpdateTreeList();
		OnSettingsChanged(m_settings);
	}

	void Settings::OnCancel() {
		FreeResources();
		DestroyWindow(Handle());
	}
}