
#pragma once

#include "../../../Resource/asd.CubemapTexture_Imp.h"
#include <array>

namespace asd
{
	class CubemapTexture_Imp_DX11
		: public CubemapTexture_Imp
	{
	private:
		ar::CubemapTexture*					rhi = nullptr;

		ID3D11Resource*	m_texture;
		ID3D11ShaderResourceView*	m_textureSRV;
		
		std::array<std::vector<ID3D11RenderTargetView*>,6>		m_textureRTVs;

		CubemapTexture_Imp_DX11(Graphics* graphics, TextureFormat format, ID3D11Resource* texture, ID3D11ShaderResourceView* textureSRV, std::array<std::vector<ID3D11RenderTargetView*>, 6>& textureRTVs, Vector2DI size, int32_t mipmapCount);
		virtual ~CubemapTexture_Imp_DX11();

	public:
		ID3D11RenderTargetView* GetRenderTargetView(int32_t direction, int32_t mipmap);

		ID3D11ShaderResourceView* GetShaderResourceView() { return m_textureSRV; }
		
		static CubemapTexture_Imp* Create(Graphics_Imp* graphics, const achar* front, const achar* left, const achar* back, const achar* right, const achar* top, const achar* bottom);

		static CubemapTexture_Imp* Create(Graphics_Imp* graphics, const achar* path, int32_t mipmapCount);

		static CubemapTexture_Imp* Create(Graphics_Imp* graphics, const achar* path);

		ar::CubemapTexture* GetRHI() const { return rhi; }
	};
}