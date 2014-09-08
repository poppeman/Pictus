#ifndef RENDER_H
#define RENDER_H

#include "types.h"

namespace Img {
	void RenderToBuffer(Filter::FilterBuffer& dest, const Filter::FilterBuffer& source, Format fmt, const Geom::RectInt& region, const Properties& props);
}

#endif
