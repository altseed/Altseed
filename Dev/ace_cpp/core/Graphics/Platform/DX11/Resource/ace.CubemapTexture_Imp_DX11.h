
#pragma once

#include "../../../Resource/ace.CubemapTexture_Imp.h"
#include <array>

namespace ace
{
	class CubemapTexture_Imp_DX11
		: public CubemapTexture_Imp
	{
	private:
		ID3D11Texture2D*	m_texture;
		ID3D11ShaderResourceView*	m_textureSRV;
		
		std::array<std::vector<ID3D11RenderTargetView*>,6>		m_textureRTVs;

		CubemapTexture_Imp_DX11(Graphics* graphics, ID3D11Texture2D* texture, ID3D11ShaderResourceView* textureSRV, std::array<std::vector<ID3D11RenderTargetView*>, 6>& textureRTVs);
		virtual ~CubemapTexture_Imp_DX11();

	public:

		ID3D11ShaderResourceView* GetShaderResourceView() { return m_textureSRV; }
		
		static CubemapTexture_Imp* Create(Graphics_Imp* graphics, const achar* front, const achar* left, const achar* back, const achar* right, const achar* top, const achar* bottom);
	};
}