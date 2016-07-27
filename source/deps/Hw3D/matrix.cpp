#include <cstring>
#include "matrix.h"

namespace Hw3D
{
	using Geom::RectFloat;

	Matrix OrthographicProjection(RectFloat zoomedArea)
	{
		Matrix proj;
		memset(&proj, 0, sizeof(proj));
		proj.m[0][0] = 2.0f / (zoomedArea.Right() - zoomedArea.Left());
		proj.m[1][1] = 2.0f / (zoomedArea.Top() - zoomedArea.Bottom());
		proj.m[2][2] = 1 / (1 - 0);
		proj.m[3][0] = (zoomedArea.Left() + zoomedArea.Right()) / (zoomedArea.Left() - zoomedArea.Right());
		proj.m[3][1] = (zoomedArea.Top() + zoomedArea.Bottom()) / (zoomedArea.Bottom() - zoomedArea.Top());
		proj.m[3][2] = 0 / (0 - 1);
		proj.m[3][3] = 1;
		return proj;
	}

	Matrix Rotate(const float angle)
	{
		auto cosT = cos(angle);
		auto sinT = sin(angle);

		Matrix rot;
		memset(&rot, 0, sizeof(rot));
		rot.m[0][0] = cosT;
		rot.m[0][1] = -sinT;
		rot.m[1][0] = sinT;
		rot.m[1][1] = cosT;
		rot.m[2][2] = 1;
		rot.m[3][3] = 1;
		return rot;
	}

	Matrix Scale(const float x, const float y, const float z)
	{
		Matrix scale;
		memset(&scale, 0, sizeof(scale));
		scale.m[0][0] = x;
		scale.m[1][1] = y;
		scale.m[2][2] = z;
		scale.m[3][3] = 1;
		return scale;
	}

	Matrix Translate(float x, float y, float z)
	{
		Matrix out;
		memset(&out, 0, sizeof(out));
		out.m[0][0] = 1;
		out.m[3][0] = x;
		out.m[1][1] = 1;
		out.m[3][1] = y;
		out.m[2][2] = 1;
		out.m[3][2] = z;
		out.m[3][3] = 1;
		return out;
	}

	Matrix Identity()
	{
		Matrix out;
		memset(&out, 0, sizeof(out));
		out.m[0][0] = 1.0f;
		out.m[1][1] = 1.0f;
		out.m[2][2] = 1.0f;
		out.m[3][3] = 1.0f;
		return out;
	}

	void GenerateQuad(Geom::RectFloat position, Geom::RectFloat texCoords, Geom::SizeFloat adjustment,
					  Geom::SizeInt targetSize, Filter::RotationAngle angle, Vertex2D &a, Vertex2D &b,
					  Vertex2D &c, Vertex2D &d)
	{
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
				adjustment += Geom::SizeFloat{static_cast<float>(targetSize.Width & 1), 0};
				a.Position = {targetSize.Width - position.Left() - position.Width(), position.Top()};
				a.TexCoord = texCoords.TopRight();
				b.Position = {targetSize.Width - position.Left(), position.Top()};
				b.TexCoord = texCoords.TopLeft();
				c.Position = {targetSize.Width - position.Left() - position.Width(), position.Bottom()};
				c.TexCoord = texCoords.BottomRight();
				d.Position = {targetSize.Width - position.Left(), position.Bottom()};
				d.TexCoord = texCoords.BottomLeft();
				break;
			case Filter::RotationAngle::FlipY:
				adjustment += Geom::SizeFloat{0, static_cast<float>(targetSize.Height & 1)};
				a.Position = {position.Left(), targetSize.Height - position.Top() - position.Height()};
				a.TexCoord = texCoords.BottomLeft();
				b.Position = {position.Right(), targetSize.Height - position.Top() - position.Height()};
				b.TexCoord = texCoords.BottomRight();
				c.Position = {position.Left(), targetSize.Height - position.Top()};
				c.TexCoord = texCoords.TopLeft();
				d.Position = {position.Right(), targetSize.Height - position.Top()};
				d.TexCoord = texCoords.TopRight();
				break;
			case Filter::RotationAngle::Rotate90:
				adjustment += Geom::SizeFloat{static_cast<float>(targetSize.Width & 1), 0};
				a.Position = {targetSize.Width - position.Top() - position.Height(), position.Left()};
				a.TexCoord = texCoords.BottomLeft();
				b.Position = {targetSize.Width - position.Top(), position.Left()};
				b.TexCoord = texCoords.TopLeft();
				c.Position = {targetSize.Width - position.Top() - position.Height(), position.Right()};
				c.TexCoord = texCoords.BottomRight();
				d.Position = {targetSize.Width - position.Top(), position.Right()};
				d.TexCoord = texCoords.TopRight();
				break;
			case Filter::RotationAngle::Rotate90FlipY:
				adjustment += Geom::SizeFloat{static_cast<float>(targetSize.Width & 1),
											  static_cast<float>(targetSize.Height & 1)};
				// SCREENSPACE:
				// TopLeft vert (x, y)
				a.Position = {targetSize.Width - position.Top() - position.Height(),
							  targetSize.Height - position.Left() - position.Width()};
				a.TexCoord = texCoords.BottomRight();
				// TopRight vert (x, y)
				b.Position = {targetSize.Width - position.Top(),
							  targetSize.Height - position.Left() - position.Width()};
				b.TexCoord = texCoords.TopRight();
				c.Position = {targetSize.Width - position.Top() - position.Height(),
							  targetSize.Height - position.Left()};
				c.TexCoord = texCoords.BottomLeft();
				d.Position = {targetSize.Width - position.Top(), targetSize.Height - position.Left()};
				d.TexCoord = texCoords.TopLeft();
				break;
			case Filter::RotationAngle::Rotate180:
				adjustment += Geom::SizeFloat{static_cast<float>(targetSize.Width & 1),
											  static_cast<float>(targetSize.Height & 1)};
				a.Position = {targetSize.Width - position.Left() - position.Width(),
							  targetSize.Height - position.Top() - position.Height()};
				a.TexCoord = texCoords.BottomRight();
				b.Position = {targetSize.Width - position.Left(),
							  targetSize.Height - position.Top() - position.Height()};
				b.TexCoord = texCoords.BottomLeft();
				c.Position = {targetSize.Width - position.Left() - position.Width(),
							  targetSize.Height - position.Top()};
				c.TexCoord = texCoords.TopRight();
				d.Position = {targetSize.Width - position.Left(), targetSize.Height - position.Top()};
				d.TexCoord = texCoords.TopLeft();
				break;
			case Filter::RotationAngle::Rotate270:
				adjustment += Geom::SizeFloat{0, static_cast<float>(targetSize.Height & 1)};
				a.Position = {position.Top(), targetSize.Height - position.Left() - position.Width()};
				a.TexCoord = texCoords.TopRight();
				b.Position = {position.Bottom(), targetSize.Height - position.Left() - position.Width()};
				b.TexCoord = texCoords.BottomRight();
				c.Position = {position.Top(), targetSize.Height - position.Left()};
				c.TexCoord = texCoords.TopLeft();
				d.Position = {position.Bottom(), targetSize.Height - position.Left()};
				d.TexCoord = texCoords.BottomLeft();
				break;
			case Filter::RotationAngle::Rotate270FlipY:
				// SCREENSPACE:
				// TopLeft vert  (x, y)
				a.Position = {position.Top(), position.Left()};
				a.TexCoord = texCoords.TopLeft();
				// TopRight vert  (x, y)
				b.Position = {position.Bottom(), position.Left()};
				b.TexCoord = texCoords.BottomLeft();
				// Bottom Left (x, y)
				c.Position = {position.Top(), position.Right()};
				c.TexCoord = texCoords.TopRight();
				// Bottom Right (x, y)
				d.Position = {position.Bottom(), position.Right()};
				d.TexCoord = texCoords.BottomRight();
				break;
			default:
				DO_THROW(Err::InvalidParam, "Angle not supported");
				break;
		}
		a.Position += adjustment;
		b.Position += adjustment;
		c.Position += adjustment;
		d.Position += adjustment;
	}

}

Hw3D::Matrix operator*(const Hw3D::Matrix& lhs, const Hw3D::Matrix& rhs) {
	Hw3D::Matrix out;
	for (int y = 0; y < 4; y++) {
		for (int x = 0; x < 4; x++) {
			out.m[y][x] =
				lhs.m[y][0] * rhs.m[0][x] +
				lhs.m[y][1] * rhs.m[1][x] +
				lhs.m[y][2] * rhs.m[2][x] +
				lhs.m[y][3] * rhs.m[3][x];
		}
	}
	return out;
}
