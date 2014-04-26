
#pragma once

#include "../../Common/Resource/ace.CubemapTexture_Imp.h"

namespace ace
{
	class CubemapTexture_Imp_GL
		: public CubemapTexture_Imp
	{
	private:
		GLuint	m_cubemapTexture;

		CubemapTexture_Imp_GL(Graphics* graphics, GLuint cubemapTexture);
		virtual ~CubemapTexture_Imp_GL();
	public:

		GLuint GetBuffer() { return m_cubemapTexture; }

		static CubemapTexture_Imp* Create(Graphics_Imp* graphics, const achar* front, const achar* left, const achar* back, const achar* right, const achar* top, const achar* bottom);
	};
}