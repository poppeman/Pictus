#ifndef PICTUS_DIALOGS_ADJUST_H
#define PICTUS_DIALOGS_ADJUST_H

#include <wx/checkbox.h>
#include <wx/dialog.h>
#include <wx/slider.h>
#include <boost/signals2.hpp>
#include "app_types.h"

namespace App
{
	class Adjust:public wxDialog
	{
	public:
		boost::signals2::signal<void(int, int, int)> OnChange;

		void Brightness(int newBright);
		int Brightness() const;
		void Contrast(int newContrast);
		int Contrast() const;
		void Gamma(int newGamma);
		int Gamma() const;

		void Apply();
		void Default();

		Adjust(wxWindow* parent);

	private:
		bool isAutoProof();

		wxCheckBox *m_autoApply;
		wxSlider *m_brightness, *m_contrast, *m_gamma;
	};
}

#endif
