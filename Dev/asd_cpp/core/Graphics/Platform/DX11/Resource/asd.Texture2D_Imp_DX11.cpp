
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "asd.Texture2D_Imp_DX11.h"
#include "../asd.Graphics_Imp_DX11.h"

#include "../../../../Log/asd.Log_Imp.h"

#include "../DirectXToolKit/DDSTextureLoader.h"

#include <sstream>

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace asd {
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
	Texture2D_Imp_DX11::Texture2D_Imp_DX11(Graphics* graphics, ID3D11Texture2D* texture, ID3D11ShaderResourceView* textureSRV, Vector2DI size, TextureFormat format)
		: Texture2D_Imp(graphics)
		, m_texture(texture)
		, m_textureSRV(textureSRV)
	{
		m_format = format;
		m_size = size;
		m_resource.resize(size.X * size.Y * ImageHelper::GetPitch(m_format));

		auto g = (Graphics_Imp*) GetGraphics();
		g->IncVRAM(ImageHelper::GetVRAMSize(GetFormat(), GetSize().X, GetSize().Y));
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Texture2D_Imp_DX11::~Texture2D_Imp_DX11()
	{
		auto g = (Graphics_Imp*) GetGraphics();
		g->DecVRAM(ImageHelper::GetVRAMSize(GetFormat(), GetSize().X, GetSize().Y));

		SafeRelease(m_texture);
		SafeRelease(m_textureSRV);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	bool Texture2D_Imp_DX11::GenerateTextureFromInternal(bool isSRGB)
	{
		auto g = (Graphics_Imp_DX11*) GetGraphics();
		auto log = g->GetLog();

		ID3D11Texture2D* texture = nullptr;
		ID3D11ShaderResourceView* srv = nullptr;

		D3D11_TEXTURE2D_DESC TexDesc;
		TexDesc.Width = m_internalTextureWidth;
		TexDesc.Height = m_internalTextureHeight;
		TexDesc.MipLevels = 1;
		TexDesc.ArraySize = 1;

		if (isSRGB)
		{
			TexDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		}
		else
		{
			TexDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		}
		
		TexDesc.SampleDesc.Count = 1;
		TexDesc.SampleDesc.Quality = 0;
		TexDesc.Usage = D3D11_USAGE_DEFAULT;
		TexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		TexDesc.CPUAccessFlags = 0;
		TexDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA data;
		data.pSysMem = m_internalTextureData.data();
		data.SysMemPitch = m_internalTextureWidth * 4;
		data.SysMemSlicePitch = m_internalTextureWidth * m_internalTextureHeight * 4;

		auto hr = (g)->GetDevice()->CreateTexture2D(&TexDesc, &data, &texture);

		m_size.X = m_internalTextureWidth;
		m_size.Y = m_internalTextureHeight;

		if (isSRGB)
		{
			m_format = TextureFormat::R8G8B8A8_UNORM_SRGB;
		}
		else
		{
			m_format = TextureFormat::R8G8B8A8_UNORM;
		}

		InternalUnload();
		if (FAILED(hr))
		{
			if (log != nullptr)
			{
				log->WriteLineStrongly("DirectX11 : CreateTexture2D");
				log->WriteLine(GraphicsHelper_DX11::GetErrorMessage(g, hr).c_str());

				std::ostringstream state;
				state << "フォーマット : " << GraphicsHelper_DX11::GetFormatName(g, TexDesc.Format) << ", 横幅 : " << TexDesc.Width << ", 縦幅 : " << TexDesc.Height;
				log->WriteLine(ToAString(state.str().c_str()).c_str());
			}
			return false;
		}
			
		D3D11_SHADER_RESOURCE_VIEW_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Format = TexDesc.Format;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MostDetailedMip = 0;
		desc.Texture2D.MipLevels = TexDesc.MipLevels;

		hr = (g)->GetDevice()->CreateShaderResourceView(texture, &desc, &srv);
		if (FAILED(hr))
		{
			SafeRelease(texture);

			if (log != nullptr)
			{
				log->WriteLineStrongly("DirectX11 : CreateShaderResourceView");
				log->WriteLine(GraphicsHelper_DX11::GetErrorMessage(g, hr).c_str());

				std::ostringstream state;
				state << "フォーマット : " << GraphicsHelper_DX11::GetFormatName(g, TexDesc.Format) << ", 横幅 : " << TexDesc.Width << ", 縦幅 : " << TexDesc.Height;
				log->WriteLine(ToAString(state.str().c_str()).c_str());
			}

			return false;
		}

		m_texture = texture;
		m_textureSRV = srv;

		g->IncVRAM(ImageHelper::GetVRAMSize(GetFormat(), GetSize().X, GetSize().Y));
		return true;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Texture2D_Imp_DX11* Texture2D_Imp_DX11::Create(Graphics_Imp_DX11* graphics, uint8_t* data, int32_t size, bool isSRGB)
	{
		if (size == 0) return nullptr;

		if (ImageHelper::IsPNG(data, size))
		{
			/* ロードしてみる */
			Texture2D_Imp_DX11* texture = new Texture2D_Imp_DX11(graphics);
			if (!texture->InternalLoad(data, size, false))
			{
				SafeRelease(texture);
				return nullptr;
			}

			if (!texture->GenerateTextureFromInternal(isSRGB))
			{
				SafeRelease(texture);
				return nullptr;
			}

			/* 必要ないので消す */
			texture->InternalUnload();

			return texture;
		}
		if (ImageHelper::IsDDS(data, size))
		{
			ID3D11Resource* texture = nullptr;
			ID3D11ShaderResourceView* textureSRV = nullptr;

			auto hr = DirectX::CreateDDSTextureFromMemory(
				graphics->GetDevice(),
				data,
				size,
				&texture,
				&textureSRV);

			if (texture == nullptr || textureSRV == nullptr)
			{
				SafeRelease(texture);
				SafeRelease(textureSRV);
				return nullptr;
			}

			D3D11_SHADER_RESOURCE_VIEW_DESC desc;
			textureSRV->GetDesc(&desc);

			auto texture_ = (ID3D11Texture2D*) texture;
			D3D11_TEXTURE2D_DESC desc_;
			texture_->GetDesc(&desc_);

			if (desc.ViewDimension != D3D_SRV_DIMENSION_TEXTURE2D)
			{
				SafeRelease(texture);
				SafeRelease(textureSRV);
				return nullptr;
			}

			if (desc.Format == DXGI_FORMAT_BC1_UNORM)
			{
				return  new Texture2D_Imp_DX11(graphics, texture_, textureSRV, Vector2DI(desc_.Width, desc_.Height), TextureFormat::BC1);
			}
			else if (desc.Format == DXGI_FORMAT_BC1_UNORM_SRGB)
			{
				return  new Texture2D_Imp_DX11(graphics, texture_, textureSRV, Vector2DI(desc_.Width, desc_.Height), TextureFormat::BC1_SRGB);
			}
			else if (desc.Format == DXGI_FORMAT_BC2_UNORM)
			{
				return  new Texture2D_Imp_DX11(graphics, texture_, textureSRV, Vector2DI(desc_.Width, desc_.Height), TextureFormat::BC2);
			}
			else if (desc.Format == DXGI_FORMAT_BC2_UNORM_SRGB)
			{
				return  new Texture2D_Imp_DX11(graphics, texture_, textureSRV, Vector2DI(desc_.Width, desc_.Height), TextureFormat::BC2_SRGB);
			}
			else if (desc.Format == DXGI_FORMAT_BC3_UNORM)
			{
				return  new Texture2D_Imp_DX11(graphics, texture_, textureSRV, Vector2DI(desc_.Width, desc_.Height), TextureFormat::BC3);
			}
			else if (desc.Format == DXGI_FORMAT_BC3_UNORM_SRGB)
			{
				return  new Texture2D_Imp_DX11(graphics, texture_, textureSRV, Vector2DI(desc_.Width, desc_.Height), TextureFormat::BC3_SRGB);
			}
			else
			{
				SafeRelease(texture);
				SafeRelease(textureSRV);
				return nullptr;
			}

		}

		return nullptr;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Texture2D_Imp_DX11* Texture2D_Imp_DX11::Create(Graphics_Imp_DX11* graphics, int32_t width, int32_t height, TextureFormat format, void* data)
	{
		auto g = (Graphics_Imp_DX11*) graphics;

		ID3D11Texture2D*			texture = nullptr;
		ID3D11ShaderResourceView*	textureSRV = nullptr;
		HRESULT hr;

		D3D11_TEXTURE2D_DESC TexDesc;
		TexDesc.Width = width;
		TexDesc.Height = height;
		TexDesc.MipLevels = 1;
		TexDesc.ArraySize = 1;
		if (format == TextureFormat::R8G8B8A8_UNORM)
		{
			TexDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		}
		else if (format == TextureFormat::R16G16B16A16_FLOAT)
		{
			TexDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
		}
		else if (format == TextureFormat::R32G32B32A32_FLOAT)
		{
			TexDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		}
		else if (format == TextureFormat::R8G8B8A8_UNORM_SRGB)
		{
			TexDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		}
		else if (format == TextureFormat::R16G16_FLOAT)
		{
			TexDesc.Format = DXGI_FORMAT_R16G16_FLOAT;
		}
		else if (format == TextureFormat::R8_UNORM)
		{
			TexDesc.Format = DXGI_FORMAT_R8_UNORM;
		}
		else
		{
			return nullptr;
		}

		TexDesc.SampleDesc.Count = 1;
		TexDesc.SampleDesc.Quality = 0;
		TexDesc.Usage = D3D11_USAGE_DEFAULT;
		TexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		TexDesc.CPUAccessFlags = 0;
		TexDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = data;
		initData.SysMemPitch = width * ImageHelper::GetPitch(format);
		initData.SysMemSlicePitch = initData.SysMemPitch * height;

		hr = g->GetDevice()->CreateTexture2D(&TexDesc, data != nullptr ? &initData : nullptr , &texture);
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

		return new Texture2D_Imp_DX11(g, texture, textureSRV, Vector2DI(width, height), format);

	End:;
		SafeRelease(texture);
		SafeRelease(textureSRV);
		return nullptr;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	bool Texture2D_Imp_DX11::Save(const achar* path)
	{
		auto g = (Graphics_Imp_DX11*) GetGraphics();
		return g->SaveTexture(path, m_texture, GetSize());
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	bool Texture2D_Imp_DX11::Lock(TextureLockInfomation* info)
	{
		if (info == nullptr) return false;
		if (m_resource.size() == 0) return false;

		info->pixels = m_resource.data();
		info->pitch = ImageHelper::GetPitch(m_format);
		info->size = m_size;
		return true;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Texture2D_Imp_DX11::Unlock()
	{
		auto g = (Graphics_Imp_DX11*) GetGraphics();

		// 非ダイナミック限定
		g->GetContext()->UpdateSubresource(
			m_texture,
			0,
			NULL,
			m_resource.data(),
			m_size.X * ImageHelper::GetPitch(m_format),
			0);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Texture2D_Imp_DX11::Reload(void* data, int32_t size)
	{
		auto g = (Graphics_Imp*) GetGraphics();
		g->DecVRAM(ImageHelper::GetVRAMSize(GetFormat(), GetSize().X, GetSize().Y));

		SafeRelease(m_texture);
		SafeRelease(m_textureSRV);

		if (!InternalLoad(data, size, false))
		{
			InternalUnload();
			return;
		}
		
		GenerateTextureFromInternal(m_format == TextureFormat::R8G8B8A8_UNORM_SRGB);

		InternalUnload();
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
}