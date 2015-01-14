
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "ace.DepthBuffer_Imp_GL.h"
#include "../ace.Graphics_Imp_GL.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	DepthBuffer_Imp_GL::DepthBuffer_Imp_GL(Graphics* graphics, GLuint buffer, Vector2DI size)
		: DepthBuffer_Imp(graphics, size)
		, m_buffer(buffer)
	{
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	DepthBuffer_Imp_GL::~DepthBuffer_Imp_GL()
	{
		glDeleteTextures(1, &m_buffer);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	DepthBuffer_Imp_GL* DepthBuffer_Imp_GL::Create(Graphics* graphics, int32_t width, int32_t height)
	{
		GLuint buf;

		// ID生成
		glGenTextures(1, &buf);

		// テクスチャをバインド
		glBindTexture(GL_TEXTURE_2D, buf);

		// テクスチャ領域生成
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, nullptr);

		// テクスチャを拡大・縮小する方法の指定
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		// テクスチャをアンバインド
		glBindTexture(GL_TEXTURE_2D, 0);

		GLCheckError();

		return new DepthBuffer_Imp_GL(graphics, buf, Vector2DI(width, height));
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
}