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

	void FlipXUV(Vertex2D& a, Vertex2D& b) {
		std::swap(a.TexCoord.X, b.TexCoord.X);
	}

	void RotateQuad(Filter::RotationAngle angle, Vertex2D& a, Vertex2D& b, Vertex2D& c, Vertex2D& d) {
		switch (angle)
		{
			case Filter::RotationAngle::RotateDefault:
				break;
			case Filter::RotationAngle::FlipX:
				FlipXUV(a, b);
				FlipXUV(c, d);
				break;
			case Filter::RotationAngle::FlipY:
				break;
			case Filter::RotationAngle::Rotate90:
				break;
			case Filter::RotationAngle::Rotate180:
				break;
			case Filter::RotationAngle::Rotate270:
				break;
			case Filter::RotationAngle::RotateUndefined:
				break;
			default:
				DO_THROW(Err::InvalidParam, L"Unknown angle");
		}
	}
}
