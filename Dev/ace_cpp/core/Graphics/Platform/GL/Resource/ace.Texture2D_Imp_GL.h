#pragma once

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "../../../Resource/ace.Texture2D_Imp.h"
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
		std::vector<uint8_t>		m_resource;

		Texture2D_Imp_GL(Graphics* graphics);
		Texture2D_Imp_GL(Graphics* graphics, GLuint texture, Vector2DI size, TextureFormat format);

		virtual ~Texture2D_Imp_GL();

		bool GenerateTextureFromInternal(bool isSRGB);
	public:
		static Texture2D_Imp_GL* Create(Graphics_Imp_GL* graphics, uint8_t* data, int32_t size, bool isSRGB);

		static Texture2D_Imp_GL* Create(Graphics_Imp_GL* graphics, int32_t width, int32_t height, TextureFormat format);

		bool Save(const achar* path) override;

		bool Lock(TextureLockInfomation& info) override;

		void Unlock() override;

		void Reload(void* data, int32_t size);

		GLuint GetBuffer() { return m_texture; }
	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
}