
#include "asd.CubemapTexture_Imp_DX11.h"
#include "../asd.Graphics_Imp_DX11.h"

#include "../../../../3rdParty/DirectXToolKit/DDSTextureLoader.h"

namespace asd
{
	CubemapTexture_Imp_DX11::CubemapTexture_Imp_DX11(
		Graphics* graphics,
		ar::CubemapTexture* rhi,
		TextureFormat format,
		ID3D11Resource* texture,
		ID3D11ShaderResourceView* textureSRV, 
		std::array<std::vector<ID3D11RenderTargetView*>, 6>& textureRTVs, 
		Vector2DI size,
		int32_t mipmapCount)
		: CubemapTexture_Imp(graphics, format, size, mipmapCount)
		, rhi(rhi)
		//, m_texture(texture)
		//, m_textureSRV(textureSRV)
		//, m_textureRTVs(textureRTVs)
	{
	}

	CubemapTexture_Imp_DX11::~CubemapTexture_Imp_DX11()
	{
		//SafeRelease(m_texture);
		//SafeRelease(m_textureSRV);
		//
		//for (auto& v : m_textureRTVs)
		//{
		//	for (auto& v_ : v)
		//	{
		//		v_->Release();
		//	}
		//}

		asd::SafeDelete(rhi);
	}

	/*
	ID3D11RenderTargetView* CubemapTexture_Imp_DX11::GetRenderTargetView(int32_t direction, int32_t mipmap)
	{
		if (direction < 0) return nullptr;
		if (direction >= 6) return nullptr;
		if (mipmap < 0) return nullptr;
		if (m_textureRTVs[direction].size() <= mipmap) return nullptr;
		
		return m_textureRTVs[direction][mipmap];
	}
	*/

	CubemapTexture_Imp* CubemapTexture_Imp_DX11::Create(Graphics_Imp* graphics, const achar* front, const achar* left, const achar* back, const achar* right, const achar* top, const achar* bottom)
	{
		/*
		auto loadFile = [graphics](const achar* path, std::vector<uint8_t>& dst)-> bool
		{
			auto staticFile = graphics->GetFile()->CreateStaticFile(path);
			if (staticFile.get() == nullptr) return false;

			dst.resize(staticFile->GetSize());
			memcpy(dst.data(), staticFile->GetData(), staticFile->GetSize());

			return true;
		};

		int32_t widthes[6];
		int32_t heights[6];
		std::vector<uint8_t> fileBuffers[6];
		std::array<std::vector<ID3D11RenderTargetView*>, 6> textureRTVs;
		std::vector<D3D11_SUBRESOURCE_DATA> data;
		std::vector<uint8_t> nulldata;

		std::vector<uint8_t> buffers[6];

		const achar* pathes[] = {
			right,
			left,
			top,
			bottom,
			front,
			back,
		};

		auto g = (Graphics_Imp_DX11*) graphics;

		for (int32_t i = 0; i < 6; i++)
		{
			if (!loadFile(pathes[i], fileBuffers[i]))
			{
				return nullptr;
			}
		}

		for (int32_t i = 0; i < 6; i++)
		{
			if (ImageHelper::LoadPNGImage(fileBuffers[i].data(), fileBuffers[i].size(), false, widthes[i], heights[i], buffers[i], g->GetLog()))
			{
			}
			else
			{
				goto End;
			}
		}

		
		auto width = widthes[0];
		auto height = heights[0];

		for (int32_t i = 0; i < 6; i++)
		{
			if (widthes[i] != width) goto End;
			if (heights[i] != height) goto End;
		}

		auto mipmapCount = ImageHelper::GetMipmapCount(width, height);

		ID3D11Texture2D* texture = nullptr;
		ID3D11ShaderResourceView* srv = nullptr;

		D3D11_TEXTURE2D_DESC TexDesc;
		TexDesc.Width = width;
		TexDesc.Height = height;
		TexDesc.MipLevels = mipmapCount;
		TexDesc.ArraySize = 6;
		TexDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		TexDesc.SampleDesc.Count = 1;
		TexDesc.SampleDesc.Quality = 0;
		TexDesc.Usage = D3D11_USAGE_DEFAULT;
		TexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		TexDesc.CPUAccessFlags = 0;
		TexDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

		D3D11_SHADER_RESOURCE_VIEW_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;

		// 全てのミップマップを使用
		desc.TextureCube.MipLevels = -1;
		desc.TextureCube.MostDetailedMip = 0;

		data.resize(mipmapCount * 6);
		nulldata.resize(width * height * 4);

		for (size_t i = 0; i < nulldata.size(); i++)
		{
			nulldata[i] = 0;
		}

		for (int32_t i = 0; i < 6; i++)
		{
			for (int32_t m = 0; m < mipmapCount; m++)
			{
				auto w = width;
				auto h = height;
				ImageHelper::GetMipmapSize(m, w, h);

				auto ind = i * mipmapCount + m;

				if (m == 0)
				{
					data[ind].pSysMem = buffers[i].data();
				}
				else
				{
					data[ind].pSysMem = nulldata.data();
				}

				data[ind].SysMemPitch = w * 4;
				data[ind].SysMemSlicePitch = data[ind].SysMemPitch * h;
			}
		}

		auto hr = g->GetDevice()->CreateTexture2D(&TexDesc, data.data(), &texture);

		if (FAILED(hr))
		{
			goto End;
		}

		hr = g->GetDevice()->CreateShaderResourceView(texture, &desc, &srv);
		if (FAILED(hr))
		{
			SafeRelease(texture);
			goto End;
		}

		// ミップマップ処理
		texture->GetDesc(&TexDesc);

		for (int32_t dirNum = 0; dirNum < 6; dirNum++)
		{
			for (int32_t mip = 0; mip < TexDesc.MipLevels; mip++)
			{
				ID3D11RenderTargetView* rtv = nullptr;
				D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
				rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
				rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
				rtvDesc.Texture2DArray.ArraySize = 1;
				rtvDesc.Texture2DArray.FirstArraySlice = dirNum;
				rtvDesc.Texture2DArray.MipSlice = mip;

				hr = g->GetDevice()->CreateRenderTargetView(texture, &rtvDesc, &rtv);
				if (FAILED(hr))
				{
					goto End;
				}
				textureRTVs[dirNum].push_back(rtv);
			}
		}

		return new CubemapTexture_Imp_DX11(graphics, TextureFormat::R8G8B8A8_UNORM, texture, srv, textureRTVs, Vector2DI(width, height), mipmapCount);

	End:;

		for (auto& v : textureRTVs)
		{
			for (auto& v_ : v)
			{
				if (v_ == nullptr) continue;
				v_->Release();
			}
		}
		*/
		return nullptr;
	}


	CubemapTexture_Imp* CubemapTexture_Imp_DX11::Create(Graphics_Imp* graphics, const achar* path, int32_t mipmapCount)
	{
		/*
		auto loadFile = [graphics](const achar* path, std::vector<uint8_t>& dst)-> bool
		{
			auto staticFile = graphics->GetFile()->CreateStaticFile(path);
			if (staticFile.get() == nullptr) return false;

			dst.resize(staticFile->GetSize());
			memcpy(dst.data(), staticFile->GetData(), staticFile->GetSize());

			return true;
		};

		int32_t widthes[6];
		int32_t heights[6];
		std::array<std::vector<ID3D11RenderTargetView*>, 6> textureRTVs;
		std::vector<D3D11_SUBRESOURCE_DATA> data;

		std::vector<std::vector<uint8_t>> fileBuffers[6];
		std::vector<std::vector<uint8_t>> buffers[6];

		auto g = (Graphics_Imp_DX11*) graphics;

		for (int32_t i = 0; i < 6; i++)
		{
			fileBuffers[i].resize(mipmapCount);
			buffers[i].resize(mipmapCount);
		}

		for (int32_t m = 0; m < mipmapCount; m++)
		{
			for (int32_t i = 0; i < 6; i++)
			{
				auto path_ = astring(path);
				auto indStr = ToAString(std::to_string(i).c_str());
				auto mipStr = ToAString(std::to_string(m).c_str());

				auto path__ = path_ + ToAString("_m") + mipStr + ToAString("_c") + indStr + ToAString(".png");

				if (!loadFile(path__.c_str(), fileBuffers[i][m]))
				{
					return nullptr;
				}
			}
		}

		int width = 0;
		int height = 0;

		for (int32_t m = 0; m < mipmapCount; m++)
		{
			for (int32_t i = 0; i < 6; i++)
			{
				if (ImageHelper::LoadPNGImage(fileBuffers[i][m].data(), fileBuffers[i].size(), false, widthes[i], heights[i], buffers[i][m], g->GetLog()))
				{
					if (m == 0)
					{
						width = widthes[0];
						height = heights[0];
					}
				}
				else
				{
					goto End;
				}
			}
		}

		ID3D11Texture2D* texture = nullptr;
		ID3D11ShaderResourceView* srv = nullptr;

		D3D11_TEXTURE2D_DESC TexDesc;
		TexDesc.Width = width;
		TexDesc.Height = height;
		TexDesc.MipLevels = mipmapCount;
		TexDesc.ArraySize = 6;
		TexDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		TexDesc.SampleDesc.Count = 1;
		TexDesc.SampleDesc.Quality = 0;
		TexDesc.Usage = D3D11_USAGE_DEFAULT;
		TexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		TexDesc.CPUAccessFlags = 0;
		TexDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

		D3D11_SHADER_RESOURCE_VIEW_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;

		// 全てのミップマップを使用
		desc.TextureCube.MipLevels = mipmapCount;
		desc.TextureCube.MostDetailedMip = 0;

		data.resize(mipmapCount * 6);

		for (int32_t i = 0; i < 6; i++)
		{
			for (int32_t m = 0; m < mipmapCount; m++)
			{
				auto w = width;
				auto h = height;
				ImageHelper::GetMipmapSize(m, w, h);

				auto ind = i * mipmapCount + m;
				data[ind].pSysMem = buffers[i][m].data();

				data[ind].SysMemPitch = w * 4;
				data[ind].SysMemSlicePitch = data[ind].SysMemPitch * h;
			}
		}

		auto hr = g->GetDevice()->CreateTexture2D(&TexDesc, data.data(), &texture);

		if (FAILED(hr))
		{
			goto End;
		}

		hr = g->GetDevice()->CreateShaderResourceView(texture, &desc, &srv);
		if (FAILED(hr))
		{
			SafeRelease(texture);
			goto End;
		}

		// ミップマップ処理
		texture->GetDesc(&TexDesc);

		for (int32_t dirNum = 0; dirNum < 6; dirNum++)
		{
			for (int32_t mip = 0; mip < TexDesc.MipLevels; mip++)
			{
				ID3D11RenderTargetView* rtv = nullptr;
				D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
				rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
				rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
				rtvDesc.Texture2DArray.ArraySize = 1;
				rtvDesc.Texture2DArray.FirstArraySlice = dirNum;
				rtvDesc.Texture2DArray.MipSlice = mip;

				hr = g->GetDevice()->CreateRenderTargetView(texture, &rtvDesc, &rtv);
				if (FAILED(hr))
				{
					goto End;
				}
				textureRTVs[dirNum].push_back(rtv);
			}
		}

		return new CubemapTexture_Imp_DX11(graphics, TextureFormat::R8G8B8A8_UNORM, texture, srv, textureRTVs, Vector2DI(width, height), mipmapCount);

	End:;

		for (auto& v : textureRTVs)
		{
			for (auto& v_ : v)
			{
				if (v_ == nullptr) continue;
				v_->Release();
			}
		}
		*/

		return nullptr;
	}


	CubemapTexture_Imp* CubemapTexture_Imp_DX11::Create(Graphics_Imp* graphics, const achar* path)
	{
		auto staticFile = graphics->GetFile()->CreateStaticFile(path);
		if (staticFile.get() == nullptr) return nullptr;

		auto g = (Graphics_Imp_DX11*)graphics;

		auto rhi = ar::CubemapTexture::Create(g->GetRHI());
		if (rhi->Initialize(g->GetRHI(), (void*)staticFile->GetBuffer().data(), staticFile->GetBuffer().size()))
		{
			return new CubemapTexture_Imp_DX11(
				graphics,
				rhi,
				(asd::TextureFormat)rhi->GetFormat(),
				nullptr,
				nullptr,
				std::array<std::vector<ID3D11RenderTargetView*>, 6>(),
				Vector2DI(rhi->GetWidth(), rhi->GetHeight()),
				rhi->GetMipmapCount());
		}

		asd::SafeDelete(rhi);

		return nullptr;
		/*
		auto staticFile = graphics->GetFile()->CreateStaticFile(path);
		if (staticFile.get() == nullptr) return nullptr;

		if (!ImageHelper::IsDDS(staticFile->GetBuffer().data(), staticFile->GetBuffer().size())) return nullptr;

		auto g = (Graphics_Imp_DX11*) graphics;

		ID3D11Resource* texture = nullptr;
		ID3D11ShaderResourceView* textureSRV = nullptr;

		auto hr = DirectX::CreateDDSTextureFromMemory(
			g->GetDevice(),
			staticFile->GetBuffer().data(),
			staticFile->GetBuffer().size(),
			&texture,
			&textureSRV);

		if (FAILED(hr)) return nullptr;

		D3D11_SHADER_RESOURCE_VIEW_DESC cubeDesc;

		textureSRV->GetDesc(&cubeDesc);

		if (cubeDesc.ViewDimension != D3D_SRV_DIMENSION_TEXTURECUBE)
		{
			SafeRelease(texture);
			SafeRelease(textureSRV);
			return nullptr;
		}

		ID3D11Texture2D* texture_ = (ID3D11Texture2D*) texture;
		D3D11_TEXTURE2D_DESC desc;
		texture_->GetDesc(&desc);
		
		return new CubemapTexture_Imp_DX11(
			graphics, 
			GraphicsHelper_DX11::GetTextureFormat(desc.Format),
			texture, 
			textureSRV, 
			std::array<std::vector<ID3D11RenderTargetView*>, 6>(), 
			Vector2DI(desc.Width, desc.Height), 
			cubeDesc.TextureCube.MipLevels);
		*/
	}
}