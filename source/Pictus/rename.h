#ifndef RENAME_H
#define RENAME_H

#include "dialog.h"
#include "ctrl_editbox.h"

namespace App {
	class Rename:public Win::Dialog {
	public:
		const std::string& Name();

		Rename(const std::string& currname);

	private:
		Win::EditBox* m_filename;

	private:
		bool PerformOnInitDialog();
		void OnOk();
		void OnCancel();

		std::string m_name;
	};
}

#endif
