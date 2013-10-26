#pragma once

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "../../Common/Resource/ace.Texture2D_Imp.h"
#include "../ace.GL.Base.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	class Texture2D_Imp_GL
		: public Texture2D_Imp
	{
	private:
		GLuint	m_texture;

		Texture2D_Imp_GL(Graphics* graphics);
		virtual ~Texture2D_Imp_GL();

		bool GenerateTextureFromInternal();
	public:
		static Texture2D_Imp_GL* Create(Graphics_Imp_GL* graphics, uint8_t* data, int32_t size);

		void Reload(void* data, int32_t size);

		GLuint GetBuffer() { return m_texture; }
	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
}