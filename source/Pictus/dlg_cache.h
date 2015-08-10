#ifndef DLG_CACHE_H
#define DLG_CACHE_H

#include "settings_page.h"
#include "ctrl_editbox.h"

namespace App {
	class SetPageCache:public App::SettingsPage {
	public:
		bool IsRootPage() { return false; }

		SetPageCache();

	private:
		bool PerformOnInitPage() override;
		void PerformUpdateFromSettings(const Reg::Settings& settings) override;
		void onWriteSettings(Reg::Settings& settings) override;

		void UpdateControls();

		Win::EditBox* m_cacheSize;
	};
}

#endif
