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
		eTextureFormat				m_format;
		std::vector<uint8_t>		m_resource;

		Texture2D_Imp_DX11(Graphics* graphics);
		Texture2D_Imp_DX11(Graphics* graphics, ID3D11Texture2D* texture, ID3D11ShaderResourceView* textureSRV, Vector2DI size, eTextureFormat format);

		virtual ~Texture2D_Imp_DX11();

		bool GenerateTextureFromInternal();
	public:
		static Texture2D_Imp_DX11* Create(Graphics_Imp_DX11* graphics, uint8_t* data, int32_t size);

		static Texture2D_Imp_DX11* Create(Graphics_Imp_DX11* graphics, int32_t width, int32_t height, eTextureFormat format);

		bool Save(const achar* path) override;

		bool Lock(TextureLockInfomation& info) override;

		void Unlock() override;

		void Reload(void* data, int32_t size);

		ID3D11ShaderResourceView* GetShaderResourceView() { return m_textureSRV; }
	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
}