#ifndef PICTUS_RENDER_GEOMETRY_H
#define PICTUS_RENDER_GEOMETRY_H

#include "orz/geom.h"
#include "illa/types.h"
#include "hw3d_vertex2d.h"

namespace App
{
	void GenerateQuad(Geom::RectFloat position, Geom::RectFloat texCoords, Geom::SizeFloat adjustment, Geom::SizeInt targetSize, Filter::RotationAngle angle, Hw3D::Vertex2D& a, Hw3D::Vertex2D& b, Hw3D::Vertex2D& c, Hw3D::Vertex2D& d);
}

#endif

