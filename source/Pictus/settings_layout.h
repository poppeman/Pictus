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
	wxSizerFlags HorizontalPadding(int ratio, bool extra=false);
	wxSizerFlags HorizontalPaddingLast(int ratio, bool extra = false);
}

#endif
