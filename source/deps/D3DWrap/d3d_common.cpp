#include "StdAfx.h"
#include "d3d_common.h"

namespace D3D {
	void GenerateQuad(Geom::RectFloat position, Geom::RectFloat texCoords, Geom::SizeFloat adjustment, Geom::SizeInt targetSize, Filter::RotationAngle angle, D3D::Vertex2D& a, D3D::Vertex2D& b, D3D::Vertex2D& c, D3D::Vertex2D& d) {
		switch (angle)
		{
			case Filter::RotationAngle::RotateDefault:
				a.Position = position.TopLeft();
				a.TexCoord = texCoords.TopLeft();
				b.Position = position.TopRight();
				b.TexCoord = texCoords.TopRight();
				c.Position = position.BottomLeft();
				c.TexCoord = texCoords.BottomLeft();
				d.Position = position.BottomRight();
				d.TexCoord = texCoords.BottomRight();
				break;
			case Filter::RotationAngle::FlipX:
				a.Position = { targetSize.Width - position.Left() - position.Width(), position.Top() };
				a.TexCoord = texCoords.TopRight();
				b.Position = { targetSize.Width - position.Left(), position.Top() };
				b.TexCoord = texCoords.TopLeft();
				c.Position = { targetSize.Width - position.Left() - position.Width(), position.Bottom() };
				c.TexCoord = texCoords.BottomRight();
				d.Position = { targetSize.Width - position.Left(), position.Bottom() };
				d.TexCoord = texCoords.BottomLeft();
				break;
			case Filter::RotationAngle::FlipY:
				a.Position = { position.Left(), targetSize.Height - position.Top() - position.Height() };
				a.TexCoord = texCoords.BottomLeft();
				b.Position = { position.Right(), targetSize.Height - position.Top() - position.Height() };
				b.TexCoord = texCoords.BottomRight();
				c.Position = { position.Left(), targetSize.Height - position.Top() };
				c.TexCoord = texCoords.TopLeft();
				d.Position = { position.Right(), targetSize.Height - position.Top() };
				d.TexCoord = texCoords.TopRight();
				break;
			case Filter::RotationAngle::Rotate90:
				a.Position = { targetSize.Width - position.Top() - position.Height(), position.Left() };
				a.TexCoord = texCoords.BottomLeft();
				b.Position = { targetSize.Width - position.Top(), position.Left() };
				b.TexCoord = texCoords.TopLeft();
				c.Position = { targetSize.Width - position.Top() - position.Height(), position.Right() };
				c.TexCoord = texCoords.BottomRight();
				d.Position = { targetSize.Width - position.Top(), position.Right() };
				d.TexCoord = texCoords.TopRight();
				break;
			case Filter::RotationAngle::Rotate180:
				a.Position = { targetSize.Width - position.Left() - position.Width(), targetSize.Height - position.Top() - position.Height() };
				a.TexCoord = texCoords.BottomRight();
				b.Position = { targetSize.Width - position.Left(), targetSize.Height - position.Top() - position.Height() };
				b.TexCoord = texCoords.BottomLeft();
				c.Position = { targetSize.Width - position.Left() - position.Width(), targetSize.Height - position.Top() };
				c.TexCoord = texCoords.TopRight();
				d.Position = { targetSize.Width - position.Left(), targetSize.Height - position.Top() };
				d.TexCoord = texCoords.TopLeft();
				break;
			case Filter::RotationAngle::Rotate270:
				a.Position = { position.Top(), targetSize.Height - position.Left() - position.Width() };
				a.TexCoord = texCoords.TopRight();
				b.Position = { position.Bottom(), targetSize.Height - position.Left() - position.Width() };
				b.TexCoord = texCoords.BottomRight();
				c.Position = { position.Top(), targetSize.Height - position.Left() };
				c.TexCoord = texCoords.TopLeft();
				d.Position = { position.Bottom(), targetSize.Height - position.Left() };
				d.TexCoord = texCoords.BottomLeft();
				break;
			case Filter::RotationAngle::RotateUndefined:
				break;
			default:
				break;
		}
		a.Position += adjustment;
		b.Position += adjustment;
		c.Position += adjustment;
		d.Position += adjustment;
	}
}
