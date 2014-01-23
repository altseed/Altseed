
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "ace.RenderTexture_Imp_GL.h"
#include "../ace.Graphics_Imp_GL.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	RenderTexture_Imp_GL::RenderTexture_Imp_GL(Graphics* graphics, GLuint texture, Vector2DI size)
		: RenderTexture_Imp(graphics, size)
		, m_texture(texture)
	{

	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	RenderTexture_Imp_GL::~RenderTexture_Imp_GL()
	{
		glDeleteTextures(1, &m_texture);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	RenderTexture_Imp_GL* RenderTexture_Imp_GL::Create(Graphics* graphics, int32_t width, int32_t height)
	{
		GLuint texture = 0;
		glGenTextures(1, &texture);

		if (glGetError() != GL_NO_ERROR) return nullptr;

		glBindTexture(GL_TEXTURE_2D, texture);

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

		glBindTexture(GL_TEXTURE_2D, 0);

		if (glGetError() != GL_NO_ERROR)
		{
			glDeleteTextures(1, &texture);
			return nullptr;
		}

		GLCheckError();

		return new RenderTexture_Imp_GL(graphics, texture, Vector2DI(width, height));
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	bool RenderTexture_Imp_GL::Save(const achar* path)
	{
		auto g = (Graphics_Imp_GL*) GetGraphics();
		return g->SaveTexture(path, m_texture, GetSize());
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
}