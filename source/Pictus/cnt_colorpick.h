#ifndef CNT_COLORPICK_H
#define CNT_COLORPICK_H

#include <wx/panel.h>
#include <illa/color.h>
#include <orz/geom.h>

wxDECLARE_EVENT(COLOR_CHANGED, wxCommandEvent);

namespace App
{
	// TODO: Make DPI aware
	class ControlColorPicker:public wxPanel
	{
	public:
		static const wchar_t* ClassName;

		enum
		{
			CursorSpace = 2,
			CursorLength = 6,
		};

	public:
		ControlColorPicker(wxWindow *parent, wxWindowID winid, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize);

		void SetRgb(Img::Color col);
		Img::Color GetRgb();

		void SetHls(Img::HLSTriplet col);
		Img::HLSTriplet GetHls() const;

	private:
		void OnPaint(wxPaintEvent& paintEvent);

		void HandleMouseMove(wxMouseEvent& e);
		void HandleMouseClick(wxMouseEvent& e);

		void CreateColorBitmap();
		void SetColorMapPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b);

		void SetCursorPosition(const Geom::PointInt &pos);

		void NotifyChange();

		// Color in HLS and RGB version
		Img::HLSTriplet m_colorHLS;
		Img::Color m_colorRGB;

		wxImage m_colorMap;
		bool m_useRGB;

		DECLARE_EVENT_TABLE()
	};
}

#endif
