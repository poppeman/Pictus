#include "cnt_colorpick.h"
#include "wintypes.h"
#include <wx/dcclient.h>

wxDEFINE_EVENT(COLOR_CHANGED, wxCommandEvent);

namespace App
{
	using Img::Color;

	BEGIN_EVENT_TABLE(ControlColorPicker, wxPanel)
			EVT_MOTION(ControlColorPicker::HandleMouseMove)
			EVT_LEFT_DOWN(ControlColorPicker::HandleMouseClick)
			EVT_PAINT(ControlColorPicker::OnPaint)
	END_EVENT_TABLE()

	ControlColorPicker::ControlColorPicker(wxWindow*parent, wxWindowID winid, const wxPoint &pos, const wxSize &size):
		wxPanel{parent, winid, pos, size},
		m_useRGB{false}
	{
		CreateColorBitmap();

		auto sizeEvent = [=](wxSizeEvent& evt) { CreateColorBitmap(); Refresh(false); };
		Bind(wxEVT_SIZING, sizeEvent);
		Bind(wxEVT_SIZE, sizeEvent);
		Bind(wxEVT_ERASE_BACKGROUND, [=](wxEraseEvent& evt) { });
	}

	void ControlColorPicker::OnPaint(wxPaintEvent &paintEvent)
	{
		wxPaintDC dc(this);
		dc.DrawBitmap(wxBitmap(m_colorMap), 0, 0, false);
	}


	/*LRESULT ControlColorPicker::wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		switch (msg) {
			case MsgSetHue:			// Change hue
				m_useRGB		= false;
				m_colorHLS.H	= static_cast<float>(lParam);
				InvalidateRect(Handle(), 0, false);
				notifyChange();
				return 0;

			case MsgSetLuminance:	// Change luminance
				m_useRGB		= false;
				m_colorHLS.L	= lParam/100.0f;
				InvalidateRect(Handle(), 0, false);
				notifyChange();
				return 0;

			case MsgSetSaturation:	// Change saturation
				m_useRGB=false;
				m_colorHLS.S=lParam/100.0f;;
				CreateColorBitmap();
				InvalidateRect(Handle(), 0, false);
				notifyChange();
				return 0;

			case MsgSetRGB:			// Set RGB color
				{
					SetRGB(static_cast<DWORD>(wParam));
					InvalidateRect(Handle(), 0, false);
					notifyChange();
					return 0;
			   }
			case MsgGetRGB:	// Get RGB color
				return GetRGB();

			case MsgGetHue:
				return static_cast<LRESULT>(m_colorHLS.H);

			case MsgGetLuminance:
				return static_cast<LRESULT>((m_colorHLS.L + 0.005f) * 100.0f);

			case MsgGetSaturation:
				return static_cast<LRESULT>((m_colorHLS.S + 0.005f) * 100.0f);
		}
		return baseWndProc(hwnd, msg, wParam, lParam, true);
	}
*/
	void ControlColorPicker::HandleMouseClick(wxMouseEvent &evt)
	{
		SetCursorPosition(Win::wxToPoint(evt.GetPosition()));
	}

	void ControlColorPicker::HandleMouseMove(wxMouseEvent &evt)
	{
		if (evt.LeftIsDown())
		{
			SetCursorPosition(Win::wxToPoint(evt.GetPosition()));
		}
	}

	void ControlColorPicker::CreateColorBitmap()
	{
		int w = GetSize().x;
		int h = GetSize().y;
		auto pData = static_cast<unsigned char*>(malloc(w * h * 3));

		int index = 0;
		for (int y = 0; y < h; ++y)
		{
			for (int x = 0; x < w; ++x)
			{
				auto color = Img::HLS_RGB(static_cast<float>(Img::HueCap) * x / w, static_cast<float>(Img::LumCap) * y / h, m_colorHLS.S);
				pData[index++] = (unsigned char) color.R;
				pData[index++] = (unsigned char) color.G;
				pData[index++] = (unsigned char) color.B;
			}
		}

		m_colorMap = wxImage(w, h, &pData[0]);

		// Draw cursor
		Geom::PointInt m_pos((int) (m_colorHLS.H * (w / 360.0f)), (int) (m_colorHLS.L * h));
		for (int x = 0; x < CursorLength; x++)
		{
			SetColorMapPixel(x + m_pos.X - CursorSpace - CursorLength, m_pos.Y, 255, 255, 0);
		}
		for (int x = 0; x < CursorLength; x++)
		{
			SetColorMapPixel(1 + x + m_pos.X + CursorSpace, m_pos.Y, 255, 255, 0);
		}
		for (int y = 0; y < CursorLength; y++)
		{
			SetColorMapPixel(m_pos.X, y + m_pos.Y - CursorSpace - CursorLength, 255, 255, 0);
		}
		for (int y = 0; y < CursorLength; y++)
		{
			SetColorMapPixel(m_pos.X, 1 + y + m_pos.Y + CursorSpace, 255, 255, 0);
		}
	}

	void ControlColorPicker::SetColorMapPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b)
	{
		if (x < 0 || y < 0) return;
		auto sz = m_colorMap.GetSize();
		if (x >= sz.x || y >= sz.y) return;

		m_colorMap.SetRGB(x, y, r, g, b);
	}

	void ControlColorPicker::SetRgb(Img::Color col)
	{
		m_colorRGB = col;
		m_useRGB = true;
		m_colorHLS = Img::RGB_HLS(m_colorRGB.R, m_colorRGB.G, m_colorRGB.B);
		CreateColorBitmap();    // Needs to be updated (saturation might have been changed)
		Refresh();
		NotifyChange();
	}

	Img::Color ControlColorPicker::GetRgb()
	{
		if (m_useRGB)
		{
			return m_colorRGB;
		}

		return Img::HLS_RGB(m_colorHLS);
	}

	void ControlColorPicker::SetHls(Img::HLSTriplet col)
	{
		m_useRGB = false;
		m_colorHLS = col;
		m_colorRGB = Img::HLS_RGB(col);
		CreateColorBitmap();
		Refresh();
		NotifyChange();
	}

	Img::HLSTriplet ControlColorPicker::GetHls() const
	{
		return m_colorHLS;
	}

	void ControlColorPicker::SetCursorPosition(const Geom::PointInt &p)
	{
		int w = GetSize().x;
		int h = GetSize().y;

		m_useRGB = false;

		// Calculate hue and luminance from pixel values
		m_colorHLS.H = Img::HueCap * p.X / w;
		m_colorHLS.L = Img::LumCap * p.Y / h;

		CreateColorBitmap();
		Refresh();
		NotifyChange();
	}

	void ControlColorPicker::NotifyChange()
	{
		auto evt = new wxCommandEvent(COLOR_CHANGED, GetId());
		wxQueueEvent(this, evt);
	}
}
