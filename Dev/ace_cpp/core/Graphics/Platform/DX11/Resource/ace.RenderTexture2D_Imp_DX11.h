
#pragma once

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "../../../Resource/ace.RenderTexture2D_Imp.h"
#include "../ace.Graphics_Imp_DX11.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	class RenderTexture2D_Imp_DX11
		: public RenderTexture2D_Imp
	{
	protected:
		ID3D11Texture2D*			m_texture;
		ID3D11ShaderResourceView*	m_textureSRV;
		ID3D11RenderTargetView*		m_textureRTV;

		RenderTexture2D_Imp_DX11(Graphics* graphics, ID3D11Texture2D* texture, ID3D11ShaderResourceView* textureSRV, ID3D11RenderTargetView* textureRTV, Vector2DI size);
		virtual ~RenderTexture2D_Imp_DX11();
	public:

		static RenderTexture2D_Imp_DX11* Create(Graphics* graphics, int32_t width, int32_t height, eTextureFormat format);

		bool Save(const achar* path) override;

		bool Lock(TextureLockInfomation& info) override { return false; };

		void Unlock() override {};

		ID3D11RenderTargetView* GetRenderTargetView() { return m_textureRTV; }
		ID3D11ShaderResourceView* GetShaderResourceView() { return m_textureSRV; }
	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
}