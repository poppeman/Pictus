#ifndef DLG_CACHE_H
#define DLG_CACHE_H

#include "settings_page.h"
#include <wx/checkbox.h>
#include <wx/textctrl.h>

namespace App
{
	class SetPageCache : public App::SettingsPage
	{
	public:
		bool IsRootPage() const override;
		std::string Caption() override;

		SetPageCache(wxWindow* parent);

	private:
		void PerformUpdateFromSettings(const Reg::Settings &settings) override;
		void onWriteSettings(Reg::Settings &settings) override;

		void UpdateControls();

		void OnAutoLimitChanged(wxCommandEvent& event);

		enum {
			AutoLimitId = wxID_HIGHEST + 1,
			CacheSizeId
		};

		wxTextCtrl* m_cacheSize;
		wxCheckBox* m_autoLimit;

		DECLARE_EVENT_TABLE()
	};
}

#endif
