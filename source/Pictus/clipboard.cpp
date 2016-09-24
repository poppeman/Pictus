#include "clipboard.h"

#include "orz/geom.h"

#include <wx/image.h>
#include <wx/clipbrd.h>
#include <boost/scoped_array.hpp>

namespace App
{
	using namespace Geom;

	bool ImageToClipboard(std::shared_ptr<Img::Image> source, Img::Properties props)
	{
		if (!source.get()) return false;
		if (source->IsFinished() == false) return false;

		Geom::SizeInt size(source->GetSize());
		Img::Surface::Ptr surface = source->CurrentSurface();
		if (surface == 0)
			return false;

		// Signal an error here
		if (wxTheClipboard->Open())
		{
			unsigned char* rgb = nullptr;
			unsigned char* alpha = nullptr;
			try
			{
				// TODO: Handle rotation
				rgb = (unsigned char*)malloc(size.Width * size.Height * 3);
				if (HasAlpha(surface->GetFormat()))
				{
					alpha = (unsigned char*)malloc(size.Width * size.Height);
				}

				auto prgb = rgb;
				auto palpha = alpha;

				// wxWidgets wants the data split into RGB and alpha. This isn't very efficient, but not much to do other than hacking wxWidgets
				boost::scoped_array<uint8_t> pBuf(new uint8_t[size.Width * 4]);
				for (auto y = 0; y < size.Height; y++) {
					auto iidx = 0;
					surface->CopyRectToBuffer(
						pBuf.get(),
						size.Width * 4,
						RectInt(PointInt(0, y), SizeInt(size.Width, 1)),
						props.BackgroundColor,
						props.Brightness,
						props.Contrast,
						props.Gamma, false);
					for (auto x = 0; x < size.Width; x++)
					{
						auto r = pBuf[iidx++];
						auto g = pBuf[iidx++];
						auto b = pBuf[iidx++];
						auto a = pBuf[iidx++];
						*prgb++ = b;
						*prgb++ = g;
						*prgb++ = r;
						if (palpha != nullptr)
						{
							*palpha++ = a;
						}
					}
				}
				wxImage img({ size.Width, size.Height }, rgb, alpha, false);
				rgb = nullptr;
				alpha = nullptr;

				wxTheClipboard->SetData(new wxBitmapDataObject(img));

			}
			catch (...)
			{
				wxTheClipboard->Close();
				if (rgb != nullptr)
				{
					free(rgb);
				}
				if (alpha != nullptr)
				{
					free(alpha);
				}
				throw;
			}

			wxTheClipboard->Close();
		}
		return true;
	}
}