#include "rename.h"
#include "wintypes.h"
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/button.h>

namespace App
{
	const std::string& Rename::Name()
	{
		return m_name;
	}

	void Rename::OnOk()
	{
		auto newName = ToAString(m_filename->GetValue());
		auto changed = (m_name != newName);
		m_name = newName;
		EndModal(changed);
	}

	void Rename::OnCancel()
	{
		EndModal(0);
	}

	Rename::Rename(wxWindow* parent, const std::string& name):
		wxDialog{ parent, wxID_ANY, Win::GetStringWx(SIDRename), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE },
		m_name( name )
	{
		auto topSizer = new wxBoxSizer(wxVERTICAL);

		auto upperSizer = new wxBoxSizer(wxHORIZONTAL);
		upperSizer->Add(new wxStaticText(this, wxID_ANY, Win::GetStringWx(SIDRenameNewFilename)), wxSizerFlags(0));
		m_filename = new wxTextCtrl(this, wxID_ANY, L"", wxDefaultPosition, { 400, wxDefaultCoord }, wxTE_PROCESS_ENTER, wxDefaultValidator);
		upperSizer->Add(m_filename, wxSizerFlags(1).Border(wxLEFT, 20));

		auto lowerSizer = new wxBoxSizer(wxHORIZONTAL);
		auto okButton = new wxButton(this, wxID_OK, Win::GetStringWx(SIDDialogOK));
		okButton->Bind(wxEVT_BUTTON, [&](wxCommandEvent& evt) { OnOk();  });
		auto cancelButton = new wxButton(this, wxID_CANCEL, Win::GetStringWx(SIDDialogCancel));
		cancelButton->Bind(wxEVT_BUTTON, [&](wxCommandEvent& evt) { OnCancel(); });

		lowerSizer->Add(okButton, wxSizerFlags(0));
		lowerSizer->Add(cancelButton, wxSizerFlags(0).Border(wxLEFT, 20));

		topSizer->Add(upperSizer, wxSizerFlags(0).Border(wxALL, 10));
		topSizer->Add(lowerSizer, wxSizerFlags(0).Border(wxALL, 10).Right());


		SetSizerAndFit(topSizer);

		m_filename->SetValue(wxString::FromUTF8(m_name.c_str()));
		m_filename->Bind(wxEVT_TEXT_ENTER, [=](wxCommandEvent& evt) { OnOk(); });
	}
}
