
#pragma once

#include "../../../Resource/ace.CubemapTexture_Imp.h"
#include <array>

namespace ace
{
	class CubemapTexture_Imp_GL
		: public CubemapTexture_Imp
	{
	private:
		GLuint	m_cubemapTexture;

		Vector2DI				size;
		int32_t					mipmapCount;

		CubemapTexture_Imp_GL(Graphics* graphics, GLuint cubemapTexture, Vector2DI size, int32_t mipmapCount);
		virtual ~CubemapTexture_Imp_GL();

	public:

		Vector2DI GetSize() const { return size; }
		int32_t GetMipmapCount() const { return mipmapCount; }
		GLuint GetBuffer() { return m_cubemapTexture; }

		static CubemapTexture_Imp* Create(Graphics_Imp* graphics, const achar* front, const achar* left, const achar* back, const achar* right, const achar* top, const achar* bottom);

		static CubemapTexture_Imp* Create(Graphics_Imp* graphics, const achar* path, int32_t mipmapCount);
	};
}