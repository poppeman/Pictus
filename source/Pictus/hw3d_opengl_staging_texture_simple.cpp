#include <GL/glew.h>
#include <orz/exception.h>
#include <cstring>
#include "hw3d_opengl_staging_texture_simple.h"
#include "hw3d_opengl_common.h"

namespace Hw3D
{
	StagingTexture::Lock OpenGlStagingTextureSimple::LockRegion(const Geom::RectInt &region, bool readOnly)
	{
		if(region.Width() < 0 || region.Height() < 0)
		{
			DO_THROW(Err::InvalidParam, "Attempted to lock a region with zero width or height");
		}

		StagingTexture::Lock l;
		l.Pitch = GetSize().Width * 4;
		l.Buffer = &m_textureDataNonPBO[(region.Left() + region.Top() * GetSize().Width) * 4];

		return l;
	}

	void OpenGlStagingTextureSimple::UnlockRegion()
	{
	}

	void OpenGlStagingTextureSimple::Transfer(Geom::RectInt sourceRect, Geom::PointInt destTopLeft)
	{
		GLenum err;

		glPixelStorei(GL_UNPACK_ROW_LENGTH, GetSize().Width);
		if ((err = glGetError()) != GL_NO_ERROR)
		{
			DO_THROW(Err::CriticalError, "Failed setting transfer row pitch: " + GetGlErrorString(err));
		}
		glTexSubImage2D(
			GL_TEXTURE_2D,
			0,
			destTopLeft.X,
			destTopLeft.Y,
			sourceRect.Width(),
			sourceRect.Height(),
			GetGlFormat(D3DFormat()),
			GetGlDataType(D3DFormat()),
			&m_textureDataNonPBO[(sourceRect.Left() + sourceRect.Top() * GetSize().Width) * 4]);
		if ((err = glGetError()) != GL_NO_ERROR)
		{
			DO_THROW(Err::CriticalError, "Failed copying data to texture: " + GetGlErrorString(err));
		}
	}

	OpenGlStagingTextureSimple::OpenGlStagingTextureSimple(Geom::SizeInt dimensions, Format fmt):
		OpenGlStagingTexture(dimensions, fmt)
	{
		m_textureDataNonPBO.resize(dimensions.Width * dimensions.Height * 4);
	}

	OpenGlStagingTextureSimple::~OpenGlStagingTextureSimple()
	{
	}
}
