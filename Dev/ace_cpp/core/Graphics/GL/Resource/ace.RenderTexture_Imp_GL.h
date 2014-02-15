
#pragma once

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "../../Common/Resource/ace.RenderTexture_Imp.h"
#include "../ace.GL.Base.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	class RenderTexture_Imp_GL
		: public RenderTexture_Imp
	{
	protected:
		GLuint	m_texture;

		RenderTexture_Imp_GL(Graphics* graphics, GLuint texture, Vector2DI size);
		virtual ~RenderTexture_Imp_GL();
	public:

		static RenderTexture_Imp_GL* Create(Graphics* graphics, int32_t width, int32_t height, eTextureFormat format);

		bool Save(const achar* path) override;

		bool Lock(TextureLockInfomation& info) override { return false; };

		void Unlock() override {};

		GLuint GetBuffer() { return m_texture; }
	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
}