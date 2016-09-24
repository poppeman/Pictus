#include "settings_layout.h"

namespace App
{
	const int SizerInnerPadding = 5;
	const int ComponentPadding = 8;

	int GetPadding()
	{
		return ComponentPadding;
	}

	wxSizerFlags StaticBoxInnerPadding(int ratio)
	{
		return wxSizerFlags(ratio).Expand().Border(wxLEFT | wxRIGHT | wxBOTTOM, SizerInnerPadding);
	}
	wxSizerFlags StaticBoxOuterPadding(int ratio)
	{
		return wxSizerFlags(ratio).Expand().Border(wxBOTTOM, ComponentPadding);
	}

	wxSizerFlags ButtonToolbarPadding()
	{
		return wxSizerFlags(0).Border(wxLEFT | wxRIGHT | wxBOTTOM, SizerInnerPadding).Right();
	}

	wxSizerFlags ButtonPadding()
	{
		return wxSizerFlags(0).Border(wxLEFT, 20);
	}

	wxSizerFlags HorizontalPaddingFirst(int ratio)
	{
		return wxSizerFlags(ratio).Expand().Border(wxLEFT | wxRIGHT | wxTOP, SizerInnerPadding);
	}

	wxSizerFlags HorizontalPadding(int ratio, bool extra)
	{
		return wxSizerFlags(ratio).Expand().Border(wxLEFT | wxRIGHT, SizerInnerPadding * (extra ? 3 : 1));
	}
	wxSizerFlags HorizontalPaddingLast(int ratio, bool extra)
	{
		return wxSizerFlags(ratio).Expand().Border(wxLEFT | wxRIGHT | wxBOTTOM, SizerInnerPadding * (extra ? 3 : 1));
	}
}
