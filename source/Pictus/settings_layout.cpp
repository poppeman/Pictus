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

	wxSizerFlags HorizontalPaddingFirst(int ratio)
	{
		return wxSizerFlags(ratio).Expand().Border(wxLEFT | wxRIGHT | wxTOP, SizerInnerPadding);
	}

	wxSizerFlags HorizontalPadding(int ratio)
	{
		return wxSizerFlags(ratio).Expand().Border(wxLEFT | wxRIGHT, SizerInnerPadding);
	}
	wxSizerFlags HorizontalPaddingLast(int ratio)
	{
		return wxSizerFlags(ratio).Expand().Border(wxLEFT | wxRIGHT | wxBOTTOM, SizerInnerPadding);
	}
}
