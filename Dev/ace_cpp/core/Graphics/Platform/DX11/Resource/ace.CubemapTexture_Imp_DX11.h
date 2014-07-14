
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

		Vector2DI	size;
		int32_t		mipmapCount;

		CubemapTexture_Imp_DX11(Graphics* graphics, ID3D11Texture2D* texture, ID3D11ShaderResourceView* textureSRV, std::array<std::vector<ID3D11RenderTargetView*>, 6>& textureRTVs, Vector2DI size, int32_t mipmapCount);
		virtual ~CubemapTexture_Imp_DX11();

	public:

		Vector2DI GetSize() const { return size; }
		int32_t GetMipmapCount() const { return mipmapCount; }

		ID3D11RenderTargetView* GetRenderTargetView(int32_t direction, int32_t mipmap);

		ID3D11ShaderResourceView* GetShaderResourceView() { return m_textureSRV; }
		
		static CubemapTexture_Imp* Create(Graphics_Imp* graphics, const achar* front, const achar* left, const achar* back, const achar* right, const achar* top, const achar* bottom);
	};
}