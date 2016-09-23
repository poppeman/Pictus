#ifndef PICTUS_SETTINGS_LAYOUT_H
#define PICTUS_SETTINGS_LAYOUT_H

#include <wx/statbox.h>
#include <wx/sizer.h>

namespace App
{
	int GetPadding();

	wxSizerFlags StaticBoxInnerPadding(int ratio);
	wxSizerFlags StaticBoxOuterPadding(int ratio);

	wxSizerFlags ButtonToolbarPadding();
	wxSizerFlags ButtonPadding();

	wxSizerFlags HorizontalPaddingFirst(int ratio);
	wxSizerFlags HorizontalPadding(int ratio);
	wxSizerFlags HorizontalPaddingLast(int ratio);
}

#endif
