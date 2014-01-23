
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "ace.Texture2D_Imp_GL.h"
#include "../ace.Graphics_Imp_GL.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Texture2D_Imp_GL::Texture2D_Imp_GL(Graphics* graphics)
		: Texture2D_Imp(graphics)
		, m_texture(0)
	{
	
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Texture2D_Imp_GL::~Texture2D_Imp_GL()
	{
		glDeleteTextures(1, &m_texture);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	bool Texture2D_Imp_GL::GenerateTextureFromInternal()
	{
		GLuint texture = 0;
		glGenTextures(1, &texture);


		GLCheckError();
		if (glGetError() != GL_NO_ERROR) return false;

		glBindTexture(GL_TEXTURE_2D, texture);

		glTexImage2D(
			GL_TEXTURE_2D, 
			0, 
			GL_RGBA,
			m_internalTextureWidth, 
			m_internalTextureHeight,
			0, 
			GL_RGBA, 
			GL_UNSIGNED_BYTE, 
			m_internalTextureData);

		glBindTexture(GL_TEXTURE_2D, 0);

		if (glGetError() != GL_NO_ERROR)
		{
			glDeleteTextures(1, &texture);
			return false;
		}
		
		m_texture = texture;
		m_size.X = m_internalTextureWidth;
		m_size.Y = m_internalTextureHeight;

		return true;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Texture2D_Imp_GL* Texture2D_Imp_GL::Create(Graphics_Imp_GL* graphics, uint8_t* data, int32_t size)
	{
		if (size == 0) return nullptr;

		/* ロードしてみる */
		Texture2D_Imp_GL* texture = new Texture2D_Imp_GL(graphics);
		if (!texture->InternalLoad(data, size,true))
		{
			SafeRelease(texture);
			return nullptr;
		}

		if (!texture->GenerateTextureFromInternal())
		{
			SafeRelease(texture);
			return nullptr;
		}

		return texture;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	bool Texture2D_Imp_GL::Save(const achar* path)
	{
		auto g = (Graphics_Imp_GL*) GetGraphics();
		return g->SaveTexture(path, m_texture, GetSize());
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Texture2D_Imp_GL::Reload(void* data, int32_t size)
	{
		glDeleteTextures(1, &m_texture);

		if (!InternalLoad(data, size, false))
		{
			InternalUnload();
			return;
		}

		GenerateTextureFromInternal();

		InternalUnload();
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
}
