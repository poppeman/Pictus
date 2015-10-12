#ifndef PICTUS_SETTINGS_H
#define PICTUS_SETTINGS_H

#include "dialog.h"
#include "appreg.h"
#include <CommCtrl.h>

namespace App {
	class SettingsPage;

	class Settings:public Win::Dialog {
	public:
		boost::signals2::signal<void(Reg::Settings)> OnSettingsChanged;

		void SetSettings(Reg::Settings settings);
		Settings();

		typedef std::shared_ptr<Settings> Ptr;

	private:
		void OnOk();
		void OnApply();
		void OnCancel();

		bool PerformOnInitDialog();
		bool PerformOnNotify(DWORD id, LPNMHDR pnmh);
		void PerformOnCreate();

		void FreeResources();

		HTREEITEM AddSettingsPageRoot(const std::string& name, int index, HTREEITEM hPrev);
		HTREEITEM AddSettingsPageChild(const std::string& name, int index, HTREEITEM hRoot, HTREEITEM hPrev);

		bool UpdateTreeList();
		void updateTreeItem(HTREEITEM);

		typedef std::shared_ptr<App::SettingsPage> PagePtr;
		typedef std::vector<PagePtr> PageList;

		PageList m_pages;
		Reg::Settings m_settings;
	};
}

#endif
