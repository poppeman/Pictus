#ifndef ADJUST_H
#define ADJUST_H

#include "dialog.h"
#include "app_types.h"

namespace App {
	class Adjust:public Win::Dialog {
	public:
		boost::signals2::signal<void(int, int, int)> OnChange;

		void Brightness(int newBright);
		int Brightness() const;
		void Contrast(int newContrast);
		int Contrast() const;
		void Gamma(int newGamma);
		int Gamma() const;

		void Apply();
		void Close();
		void Default();

		Adjust();

	private:
		bool PerformOnInitDialog();
		bool PerformOnHScroll(WPARAM wp, LPARAM lp);

		bool isAutoProof();
	};
}

#endif
