
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "ace.RenderTexture_Imp_DX11.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	RenderTexture_Imp_DX11::RenderTexture_Imp_DX11(Graphics* graphics, ID3D11Texture2D* texture, ID3D11ShaderResourceView* textureSRV, ID3D11RenderTargetView* textureRTV, Vector2DI size)
		: RenderTexture_Imp(graphics, size)
		, m_texture(texture)
		, m_textureSRV(textureSRV)
		, m_textureRTV(textureRTV)
	{

	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	RenderTexture_Imp_DX11::~RenderTexture_Imp_DX11()
	{
		SafeRelease(m_texture);
		SafeRelease(m_textureSRV);
		SafeRelease(m_textureRTV);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	RenderTexture_Imp_DX11* RenderTexture_Imp_DX11::Create(Graphics* graphics, int32_t width, int32_t height)
	{
		auto g = (Graphics_Imp_DX11*) graphics;

		ID3D11Texture2D*			texture = nullptr;
		ID3D11ShaderResourceView*	textureSRV = nullptr;
		ID3D11RenderTargetView*		textureRTV = nullptr;
		HRESULT hr;

		D3D11_TEXTURE2D_DESC TexDesc;
		TexDesc.Width = width;
		TexDesc.Height = height;
		TexDesc.MipLevels = 1;
		TexDesc.ArraySize = 1;
		TexDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		TexDesc.SampleDesc.Count = 1;
		TexDesc.SampleDesc.Quality = 0;
		TexDesc.Usage = D3D11_USAGE_DEFAULT;
		TexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		TexDesc.CPUAccessFlags = 0;
		TexDesc.MiscFlags = 0;

		
		hr = g->GetDevice()->CreateTexture2D(&TexDesc, nullptr, &texture);
		if (FAILED(hr))
		{
			goto End;
		}

		D3D11_SHADER_RESOURCE_VIEW_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Format = TexDesc.Format;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MostDetailedMip = 0;
		desc.Texture2D.MipLevels = TexDesc.MipLevels;

		hr = g->GetDevice()->CreateShaderResourceView(texture, &desc, &textureSRV);
		if (FAILED(hr))
		{
			goto End;
		}

		hr = g->GetDevice()->CreateRenderTargetView(texture, NULL, &textureRTV);
		if (FAILED(hr))
		{
			goto End;
		}

		return new RenderTexture_Imp_DX11(g, texture, textureSRV, textureRTV, Vector2DI(width, height));

	End:;
		SafeRelease(texture);
		SafeRelease(textureSRV);
		SafeRelease(textureRTV);
		return nullptr;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	bool RenderTexture_Imp_DX11::Save(const achar* path)
	{
		auto g = (Graphics_Imp_DX11*) GetGraphics();
		return g->SaveTexture(path, m_texture, GetSize());
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
}