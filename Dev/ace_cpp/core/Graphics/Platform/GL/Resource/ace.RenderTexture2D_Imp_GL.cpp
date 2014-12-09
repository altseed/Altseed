
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "ace.RenderTexture2D_Imp_GL.h"
#include "../ace.Graphics_Imp_GL.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	RenderTexture2D_Imp_GL::RenderTexture2D_Imp_GL(Graphics* graphics, GLuint texture, Vector2DI size, TextureFormat format)
		: RenderTexture2D_Imp(graphics, size)
		, m_texture(texture)
	{
		m_format = format;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	RenderTexture2D_Imp_GL::~RenderTexture2D_Imp_GL()
	{
		glDeleteTextures(1, &m_texture);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	RenderTexture2D_Imp_GL* RenderTexture2D_Imp_GL::Create(Graphics* graphics, int32_t width, int32_t height, TextureFormat format)
	{
		GLuint texture = 0;
		glGenTextures(1, &texture);

		if (glGetError() != GL_NO_ERROR) return nullptr;

		glBindTexture(GL_TEXTURE_2D, texture);

		if (format == TextureFormat::R8G8B8A8_UNORM)
		{
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RGBA,
				width,
				height,
				0,
				GL_RGBA,
				GL_UNSIGNED_BYTE,
				nullptr);
		}
		else if (format == TextureFormat::R32G32B32A32_FLOAT)
		{
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RGBA32F,
				width,
				height,
				0,
				GL_RGBA,
				GL_FLOAT,
				nullptr);
		}
		else if (format == TextureFormat::R8G8B8A8_UNORM_SRGB)
		{
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_SRGB8_ALPHA8,
				width,
				height,
				0,
				GL_RGBA,
				GL_UNSIGNED_BYTE,
				nullptr);
		}
		else if (format == TextureFormat::R16G16_FLOAT)
		{
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RG16F,
				width,
				height,
				0,
				GL_RG,
				GL_FLOAT,
				nullptr);
		}
		else if (format == TextureFormat::R8_UNORM)
		{
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_R8,
				width,
				height,
				0,
#ifdef __APPLE__
				GL_RED,
#else 
				GL_R,
#endif
				GL_UNSIGNED_BYTE,
				nullptr);
		}

		glBindTexture(GL_TEXTURE_2D, 0);

		if (glGetError() != GL_NO_ERROR)
		{
			glDeleteTextures(1, &texture);
			return nullptr;
		}

		GLCheckError();

		return new RenderTexture2D_Imp_GL(graphics, texture, Vector2DI(width, height), format);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	bool RenderTexture2D_Imp_GL::Save(const achar* path)
	{
		auto g = (Graphics_Imp_GL*) GetGraphics();
		return g->SaveTexture(path, m_texture, GetSize());
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
}