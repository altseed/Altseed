
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "ace.Texture2D_Imp_DX11.h"
#include "../ace.Graphics_Imp_DX11.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Texture2D_Imp_DX11::Texture2D_Imp_DX11(Graphics* graphics)
		: Texture2D_Imp(graphics)
		, m_texture(nullptr)
		, m_textureSRV(nullptr)
	{
	
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Texture2D_Imp_DX11::~Texture2D_Imp_DX11()
	{
		SafeRelease(m_texture);
		SafeRelease(m_textureSRV);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	bool Texture2D_Imp_DX11::GenerateTextureFromInternal()
	{
		ID3D11Texture2D* texture = nullptr;
		ID3D11ShaderResourceView* srv = nullptr;

		D3D11_TEXTURE2D_DESC TexDesc;
		TexDesc.Width = m_internalTextureWidth;
		TexDesc.Height = m_internalTextureHeight;
		TexDesc.MipLevels = 1;
		TexDesc.ArraySize = 1;
		TexDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		TexDesc.SampleDesc.Count = 1;
		TexDesc.SampleDesc.Quality = 0;
		TexDesc.Usage = D3D11_USAGE_DEFAULT;
		TexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		TexDesc.CPUAccessFlags = 0;
		TexDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA data;
		data.pSysMem = m_internalTextureData;
		data.SysMemPitch = m_internalTextureWidth * 4;
		data.SysMemSlicePitch = m_internalTextureWidth * m_internalTextureHeight * 4;

		auto hr = ((Graphics_Imp_DX11*)GetGraphics())->GetDevice()->CreateTexture2D(&TexDesc, &data, &texture);

		m_size.X = m_internalTextureWidth;
		m_size.Y = m_internalTextureHeight;

		InternalUnload();
		if (FAILED(hr))
		{
			return false;
		}
			
		D3D11_SHADER_RESOURCE_VIEW_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Format = TexDesc.Format;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MostDetailedMip = 0;
		desc.Texture2D.MipLevels = TexDesc.MipLevels;

		hr = ((Graphics_Imp_DX11*) GetGraphics())->GetDevice()->CreateShaderResourceView(texture, &desc, &srv);
		if (FAILED(hr))
		{
			SafeRelease(texture);
			return false;
		}

		m_texture = texture;
		m_textureSRV = srv;


		return true;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Texture2D_Imp_DX11* Texture2D_Imp_DX11::Create(Graphics_Imp_DX11* graphics, uint8_t* data, int32_t size)
	{
		if (size == 0) return nullptr;
		
		/* ロードしてみる */
		Texture2D_Imp_DX11* texture = new Texture2D_Imp_DX11(graphics);
		if (!texture->InternalLoad(data, size,false))
		{
			SafeRelease(texture);
			return nullptr;
		}

		if (!texture->GenerateTextureFromInternal())
		{
			SafeRelease(texture);
			return nullptr;
		}

		/* 必要ないので消す */
		texture->InternalUnload();

		return texture;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Texture2D_Imp_DX11::Reload(void* data, int32_t size)
	{
		SafeRelease(m_texture);
		SafeRelease(m_textureSRV);

		if (!InternalLoad(data, size, false))
		{
			InternalUnload();
			return;
		}
		
		GenerateTextureFromInternal();

		InternalUnload();
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
}