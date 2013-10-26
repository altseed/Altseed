#pragma once

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "../../Common/Resource/ace.Texture2D_Imp.h"
#include "../ace.DX11.Base.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	class Texture2D_Imp_DX11
		: public Texture2D_Imp
	{
	private:
		ID3D11Texture2D*	m_texture;
		ID3D11ShaderResourceView*	m_textureSRV;

		Texture2D_Imp_DX11(Graphics* graphics);
		virtual ~Texture2D_Imp_DX11();

		bool GenerateTextureFromInternal();
	public:
		static Texture2D_Imp_DX11* Create(Graphics_Imp_DX11* graphics, uint8_t* data, int32_t size);

		void Reload(void* data, int32_t size);

		ID3D11ShaderResourceView* GetShaderResourceView() { return m_textureSRV; }
	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
}