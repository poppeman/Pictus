#include "StdAfx.h"
#include "d3d_common.h"

namespace D3D {
	void GenerateQuad(Geom::RectFloat position, Geom::RectFloat texCoords, Geom::SizeFloat adjustment, D3D::Vertex2D& a, D3D::Vertex2D& b, D3D::Vertex2D& c, D3D::Vertex2D& d) {
		a.Position = position.TopLeft() + adjustment;
		a.TexCoord = texCoords.TopLeft();
		b.Position = position.TopRight() + adjustment;
		b.TexCoord = texCoords.TopRight();
		c.Position = position.BottomLeft() + adjustment;
		c.TexCoord = texCoords.BottomLeft();
		d.Position = position.BottomRight() + adjustment;
		d.TexCoord = texCoords.BottomRight();
	}
}
