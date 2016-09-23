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

		dc.SetPen(wxPen(wxColor(255, 255, 0)));

		// Draw cursor
		int w = GetSize().x;
		int h = GetSize().y;

		Geom::PointInt m_pos((int)(m_colorHLS.H * (w / 360.0f)), (int)(m_colorHLS.L * h));
		dc.DrawLine(m_pos.X - CursorSpace - CursorLength, m_pos.Y, m_pos.X - CursorSpace, m_pos.Y);
		dc.DrawLine(m_pos.X + CursorSpace + CursorLength, m_pos.Y, m_pos.X + CursorSpace, m_pos.Y);

		dc.DrawLine(m_pos.X, m_pos.Y - CursorSpace - CursorLength, m_pos.X, m_pos.Y - CursorSpace);
		dc.DrawLine(m_pos.X, m_pos.Y + CursorSpace + CursorLength, m_pos.X, m_pos.Y + CursorSpace);
	}

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

		Refresh();
		NotifyChange();
	}

	void ControlColorPicker::NotifyChange()
	{
		wxCommandEvent evt(COLOR_CHANGED, GetId());
		ProcessEvent(evt);
	}
}
