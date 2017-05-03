
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "asd.Graphics_Imp_DX11.h"
#include "../../asd.RenderingThread.h"

#include "../../../Log/asd.Log.h"
#include "../../../Window/asd.Window_Imp.h"
#include "Resource/asd.VertexBuffer_Imp_DX11.h"
#include "Resource/asd.IndexBuffer_Imp_DX11.h"
#include "Resource/asd.NativeShader_Imp_DX11.h"
#include "Resource/asd.DepthBuffer_Imp_DX11.h"

#include "Resource/asd.Texture2D_Imp_DX11.h"
#include "Resource/asd.RenderTexture2D_Imp_DX11.h"
#include "Resource/asd.CubemapTexture_Imp_DX11.h"

#include "../../../3rdParty/DirectXToolKit/DDSTextureLoader.h"

#include <sstream>

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace asd {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void GraphicsHelper_DX11::LoadTexture(Graphics_Imp_DX11* graphics, void* imgdata, int32_t width, int32_t height, ID3D11Texture2D*& texture, ID3D11ShaderResourceView*& textureSRV)
	{
		ID3D11ShaderResourceView* srv = nullptr;
		texture = nullptr;

		D3D11_TEXTURE2D_DESC TexDesc;
		TexDesc.Width = width;
		TexDesc.Height = height;
		TexDesc.MipLevels = 1;
		TexDesc.ArraySize = 1;
		TexDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		TexDesc.SampleDesc.Count = 1;
		TexDesc.SampleDesc.Quality = 0;
		TexDesc.Usage = D3D11_USAGE_DEFAULT;
		TexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		TexDesc.CPUAccessFlags = 0;
		TexDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA data;
		data.pSysMem = imgdata;
		data.SysMemPitch = width * 4;
		data.SysMemSlicePitch =width * height * 4;

		auto hr = graphics->GetDevice()->CreateTexture2D(&TexDesc, &data, &texture);

		if (FAILED(hr))
		{
			texture = nullptr;
			textureSRV = nullptr;
			return;
		}

		D3D11_SHADER_RESOURCE_VIEW_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Format = TexDesc.Format;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MostDetailedMip = 0;
		desc.Texture2D.MipLevels = TexDesc.MipLevels;

		hr = graphics->GetDevice()->CreateShaderResourceView(texture, &desc, &srv);
		if (FAILED(hr))
		{
			SafeRelease(texture);
			texture = nullptr;
			textureSRV = nullptr;
			return;
		}

		texture = texture;
		textureSRV = srv;
	}

	astring GraphicsHelper_DX11::GetErrorMessage(Graphics_Imp_DX11* graphics, HRESULT hr)
	{
		if (hr == D3D11_ERROR_FILE_NOT_FOUND) return ToAString("ファイルが見つかりませんでした。");
		if (hr == D3D11_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS) return ToAString("特定の種類のステート オブジェクトの一意のインスタンスが多すぎます。");
		if (hr == D3D11_ERROR_TOO_MANY_UNIQUE_VIEW_OBJECTS) return ToAString("特定の種類のビュー オブジェクトの一意のインスタンスが多すぎます。");
		if (hr == D3D11_ERROR_DEFERRED_CONTEXT_MAP_WITHOUT_INITIAL_DISCARD) return ToAString("D3D11_MAP_WRITE_DISCARD ではありませんでした。");

		if (hr == E_INVALIDARG) return ToAString("戻り関数に無効なパラメーターが渡されました。");
		
		if (hr == E_OUTOFMEMORY)
		{
			std::ostringstream err;

			err << "メモリが不足しています。(推定使用VRAM " << graphics->GetUsedVRAMSize() << "byte)";
			return ToAString(err.str().c_str());
		}

		return ToAString("不明なエラーです。");
	}

	std::string GraphicsHelper_DX11::GetFormatName(Graphics_Imp_DX11* graphics, DXGI_FORMAT format)
	{
		if (format == DXGI_FORMAT_R8G8B8A8_UNORM_SRGB) return std::string("DXGI_FORMAT_R8G8B8A8_UNORM_SRGB");
		if (format == DXGI_FORMAT_R8G8B8A8_UNORM) return std::string("DXGI_FORMAT_R8G8B8A8_UNORM");

		return std::string("Unknown");
	}


	TextureFormat GraphicsHelper_DX11::GetTextureFormat(DXGI_FORMAT format)
	{
		if (format == DXGI_FORMAT_R8G8B8A8_UNORM) return TextureFormat::R8G8B8A8_UNORM;
		if (format == DXGI_FORMAT_R8G8B8A8_UNORM_SRGB) return TextureFormat::R8G8B8A8_UNORM_SRGB;
		if (format == DXGI_FORMAT_R16G16B16A16_FLOAT) return TextureFormat::R16G16B16A16_FLOAT;
		if (format == DXGI_FORMAT_R32G32B32A32_FLOAT) return TextureFormat::R32G32B32A32_FLOAT;
		if (format == DXGI_FORMAT_R16G16_FLOAT) return TextureFormat::R16G16_FLOAT;
		if (format == DXGI_FORMAT_R8_UNORM) return TextureFormat::R8_UNORM;
		if (format == DXGI_FORMAT_BC1_UNORM) return TextureFormat::BC1;
		if (format == DXGI_FORMAT_BC1_UNORM_SRGB) return TextureFormat::BC1_SRGB;
		if (format == DXGI_FORMAT_BC2_UNORM) return TextureFormat::BC2;
		if (format == DXGI_FORMAT_BC2_UNORM_SRGB) return TextureFormat::BC2_SRGB;
		if (format == DXGI_FORMAT_BC3_UNORM) return TextureFormat::BC3;
		if (format == DXGI_FORMAT_BC3_UNORM_SRGB) return TextureFormat::BC3_SRGB;

		assert(0);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	class EffectTextureLoader_DX11
		: public EffectTextureLoader
	{
	public:
		EffectTextureLoader_DX11(Graphics_Imp_DX11* graphics)
			:EffectTextureLoader(graphics)
		{
		}
		virtual ~EffectTextureLoader_DX11()
		{}

	public:
		void* InternalLoadDDS(Graphics_Imp* graphics, const std::vector<uint8_t>& data)
		{
			ID3D11Resource* texture = nullptr;
			ID3D11ShaderResourceView* textureSRV = nullptr;

			auto hr = DirectX::CreateDDSTextureFromMemory(
				((Graphics_Imp_DX11*) m_graphics)->GetDevice(),
				data.data(),
				data.size(),
				&texture,
				&textureSRV);

			if (texture == nullptr || textureSRV == nullptr)
			{
				SafeRelease(texture);
				SafeRelease(textureSRV);
				return nullptr;
			}

			SafeRelease(texture);
			return textureSRV;
		}

		void* InternalLoad(Graphics_Imp* graphics, std::vector<uint8_t>& data, int32_t width, int32_t height)
		{
			ID3D11Texture2D* texture = nullptr;
			ID3D11ShaderResourceView* textureSRV = nullptr;

			GraphicsHelper_DX11::LoadTexture(
				(Graphics_Imp_DX11*) m_graphics,
				data.data(), 
				width,
				height, 
				texture, 
				textureSRV);

			SafeRelease(texture);
			return textureSRV;
		}

		void InternalUnload(void* data)
		{
			if (data != NULL)
			{
				ID3D11ShaderResourceView* texture = (ID3D11ShaderResourceView*) data;
				texture->Release();
			}
		}

		bool IsReversed()
		{
			return false;
		}
	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	class EffectModelLoader_DX11
		: public EffectModelLoader
	{
	public:
		EffectModelLoader_DX11(Graphics_Imp_DX11* graphics)
			:EffectModelLoader(graphics)
		{
		}
		virtual ~EffectModelLoader_DX11()
		{}

		void* InternalLoad(Graphics_Imp* graphics, const std::vector<uint8_t>& data) override
		{
			auto g = (Graphics_Imp_DX11*) graphics;

			HRESULT hr;

			size_t size_model = data.size();
			uint8_t* data_model = new uint8_t[size_model];
			memcpy(data_model, data.data(), size_model);

			::EffekseerRendererDX11::Model* model = new ::EffekseerRendererDX11::Model(data_model, size_model);

			model->ModelCount = Effekseer::Min(Effekseer::Max(model->GetModelCount(), 1), 40);

			model->VertexCount = model->GetVertexCount();

			if (model->VertexCount == 0) return NULL;

			{
				std::vector<Effekseer::Model::VertexWithIndex> vs;
				for (int32_t m = 0; m < model->ModelCount; m++)
				{
					for (int32_t i = 0; i < model->GetVertexCount(); i++)
					{
						Effekseer::Model::VertexWithIndex v;
						v.Position = model->GetVertexes()[i].Position;
						v.Normal = model->GetVertexes()[i].Normal;
						v.Binormal = model->GetVertexes()[i].Binormal;
						v.Tangent = model->GetVertexes()[i].Tangent;
						v.UV = model->GetVertexes()[i].UV;
						v.Index[0] = m;

						vs.push_back(v);
					}
				}

				ID3D11Buffer* vb = NULL;

				D3D11_BUFFER_DESC hBufferDesc;
				hBufferDesc.ByteWidth = sizeof(Effekseer::Model::VertexWithIndex) * model->GetVertexCount() * model->ModelCount;
				hBufferDesc.Usage = D3D11_USAGE_DEFAULT;
				hBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
				hBufferDesc.CPUAccessFlags = 0;
				hBufferDesc.MiscFlags = 0;
				hBufferDesc.StructureByteStride = sizeof(float);

				D3D11_SUBRESOURCE_DATA hSubResourceData;
				hSubResourceData.pSysMem = &(vs[0]);
				hSubResourceData.SysMemPitch = 0;
				hSubResourceData.SysMemSlicePitch = 0;

				if (FAILED(g->GetDevice()->CreateBuffer(&hBufferDesc, &hSubResourceData, &vb)))
				{
					return NULL;
				}

				model->VertexBuffer = vb;
			}

			model->FaceCount = model->GetFaceCount();

			/* 0.50より追加(0.50以前から移行する時は追記する必要あり) */
			model->IndexCount = model->FaceCount * 3;

			{
				std::vector<Effekseer::Model::Face> fs;
				for (int32_t m = 0; m < model->ModelCount; m++)
				{
					for (int32_t i = 0; i < model->FaceCount; i++)
					{
						Effekseer::Model::Face f;
						f.Indexes[0] = model->GetFaces()[i].Indexes[0] + model->GetVertexCount() * m;
						f.Indexes[1] = model->GetFaces()[i].Indexes[1] + model->GetVertexCount() * m;
						f.Indexes[2] = model->GetFaces()[i].Indexes[2] + model->GetVertexCount() * m;
						fs.push_back(f);
					}
				}

				ID3D11Buffer* ib = NULL;
				D3D11_BUFFER_DESC hBufferDesc;
				hBufferDesc.Usage = D3D11_USAGE_DEFAULT;
				hBufferDesc.ByteWidth = sizeof(int32_t) * 3 * model->FaceCount * model->ModelCount;
				hBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
				hBufferDesc.CPUAccessFlags = 0;
				hBufferDesc.MiscFlags = 0;
				hBufferDesc.StructureByteStride = sizeof(int32_t);

				D3D11_SUBRESOURCE_DATA hSubResourceData;
				hSubResourceData.pSysMem = &(fs[0]);
				hSubResourceData.SysMemPitch = 0;
				hSubResourceData.SysMemSlicePitch = 0;

				if (FAILED(g->GetDevice()->CreateBuffer(&hBufferDesc, &hSubResourceData, &ib)))
				{
					return NULL;
				}

				model->IndexBuffer = ib;
			}

			delete [] data_model;

			return (void*) model;
		}

		void InternalUnload(void* data) override
		{
			if (data != NULL)
			{
				::EffekseerRendererDX11::Model* model = (::EffekseerRendererDX11::Model*) data;
				delete model;
			}
		}
	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
		DistortingCallbackDX11::DistortingCallbackDX11(::EffekseerRendererDX11::Renderer* renderer, ID3D11Device* d3d11Device, ID3D11DeviceContext* d3d11Context)
			: renderer(renderer)
			, g_D3d11Device(d3d11Device)
			, g_D3d11Context(d3d11Context)
		{
		}

		DistortingCallbackDX11::~DistortingCallbackDX11()
		{
			ReleaseTexture();
		}

		void DistortingCallbackDX11::ReleaseTexture()
		{
			ES_SAFE_RELEASE(backGroundTextureSRV);
			ES_SAFE_RELEASE(backGroundTexture);
		}

		void DistortingCallbackDX11::PrepareTexture(uint32_t width, uint32_t height, DXGI_FORMAT format)
		{
			ReleaseTexture();

			ZeroMemory(&backGroundTextureDesc, sizeof(backGroundTextureDesc));
			backGroundTextureDesc.Usage = D3D11_USAGE_DEFAULT;
			backGroundTextureDesc.Format = format;
			backGroundTextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			backGroundTextureDesc.Width = width;
			backGroundTextureDesc.Height = height;
			backGroundTextureDesc.CPUAccessFlags = 0;
			backGroundTextureDesc.MipLevels = 1;
			backGroundTextureDesc.ArraySize = 1;
			backGroundTextureDesc.SampleDesc.Count = 1;
			backGroundTextureDesc.SampleDesc.Quality = 0;

			HRESULT hr = S_OK;
			hr = g_D3d11Device->CreateTexture2D(&backGroundTextureDesc, nullptr, &backGroundTexture);
			if (FAILED(hr)) {
				return;
			}

			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
			ZeroMemory(&srvDesc, sizeof(srvDesc));
			switch (format)
			{
			case DXGI_FORMAT_R8G8B8A8_TYPELESS:
				srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
				break;
			case DXGI_FORMAT_R16G16B16A16_TYPELESS:
				srvDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
				break;
			default:
				srvDesc.Format = format;
				break;
			}
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MipLevels = 1;
			hr = g_D3d11Device->CreateShaderResourceView(backGroundTexture, &srvDesc, &backGroundTextureSRV);
			if (FAILED(hr)) {
				return;
			}
		}

		void DistortingCallbackDX11::OnDistorting()
		{
			if (!IsEnabled)
			{
				renderer->SetBackground(nullptr);
				return;
			}

			HRESULT hr = S_OK;

			ID3D11RenderTargetView* renderTargetView = nullptr;
			ID3D11Texture2D* renderTexture = nullptr;

			g_D3d11Context->OMGetRenderTargets(1, &renderTargetView, nullptr);
			renderTargetView->GetResource(reinterpret_cast<ID3D11Resource**>(&renderTexture));

			// レンダーターゲット情報を取得
			D3D11_TEXTURE2D_DESC renderTextureDesc;
			renderTexture->GetDesc(&renderTextureDesc);

			// シザリング範囲を取得
			UINT numScissorRects = 1;
			D3D11_RECT scissorRect;
			g_D3d11Context->RSGetScissorRects(&numScissorRects, &scissorRect);

			// 描画範囲を計算
			uint32_t width = renderTextureDesc.Width;
			uint32_t height = renderTextureDesc.Height;
			if (numScissorRects > 0) {
				width = scissorRect.right - scissorRect.left;
				height = scissorRect.bottom - scissorRect.top;
			}

			// 保持テクスチャとフォーマットが異なればテクスチャを作り直す
			if (backGroundTextureSRV == nullptr ||
				backGroundTextureDesc.Width != width ||
				backGroundTextureDesc.Height != height ||
				backGroundTextureDesc.Format != renderTextureDesc.Format)
			{
				PrepareTexture(width, height, renderTextureDesc.Format);
			}

			if (width == renderTextureDesc.Width &&
				height == renderTextureDesc.Height)
			{
				// 背景テクスチャへコピー
				g_D3d11Context->CopyResource(backGroundTexture, renderTexture);
			}
			else
			{
				// 背景テクスチャへ部分的コピー
				D3D11_BOX srcBox;
				srcBox.left = scissorRect.left;
				srcBox.top = scissorRect.top;
				srcBox.right = scissorRect.right;
				srcBox.bottom = scissorRect.bottom;
				srcBox.front = 0;
				srcBox.back = 1;
				g_D3d11Context->CopySubresourceRegion(backGroundTexture, 0,
					0, 0, 0, renderTexture, 0, &srcBox);
			}

			// 取得したリソースの参照カウンタを下げる
			ES_SAFE_RELEASE(renderTexture);
			ES_SAFE_RELEASE(renderTargetView);

			renderer->SetBackground(backGroundTextureSRV);
		}
	

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Graphics_Imp_DX11::Graphics_Imp_DX11(
	ar::Manager* manager,
	Window* window,
	Vector2DI size,
	Log* log,
	File* file,
	GraphicsOption option)
	: Graphics_Imp(manager, size, log,file, option)
	, m_window(window)
{
	SafeAddRef(window);

#if !defined(_CONSOLE_GAME)
	m_renderingThread->Run(this, StartRenderingThreadFunc, EndRenderingThreadFunc);
#endif

	if (GetRHI()->GetDeviceType() == ar::GraphicsDeviceType::DirectX11)
	{
		GetEffectSetting()->SetTextureLoader(new EffectTextureLoader_DX11(this));
		GetEffectSetting()->SetModelLoader(new EffectModelLoader_DX11(this));

		auto flevel = GetDevice()->GetFeatureLevel();

		if (flevel == D3D_FEATURE_LEVEL_9_3 ||
			flevel == D3D_FEATURE_LEVEL_9_2 ||
			flevel == D3D_FEATURE_LEVEL_9_1)
		{
			isInitializedAsDX9 = true;
		}
	}
	
	rhiContext = ar::Context::Create(GetRHI());
	rhiContext->Initialize(GetRHI());

	renderTargets.fill(0);
	currentRenderTargets.fill(0);
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Graphics_Imp_DX11::~Graphics_Imp_DX11()
{
#if !defined(_CONSOLE_GAME)
	m_renderingThread->AddEvent(nullptr);
	while (m_renderingThread->IsRunning())
	{
		Sleep(1);
	}
	m_renderingThread.reset();
#endif

	for (auto& r : currentRenderTargets)
	{
		SafeRelease(r);
	}
	SafeRelease(currentDepthTarget);

	for (auto& r : renderTargets)
	{
		SafeRelease(r);
	}
	SafeRelease(depthTarget);

	asd::SafeDelete(rhiContext);
	SafeRelease(m_window);

}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Graphics_Imp_DX11::WriteAdapterInformation(Log* log, IDXGIAdapter1* adapter, int32_t index)
{
	if (log == nullptr) return;

	auto write = [log](std::ostringstream& os) -> void
	{
		log->Write(ToAString(os.str().c_str()).c_str());
	};

	auto writeTable = [log, write](const char* title, std::ostringstream& text, bool isLast) -> void
	{
		log->Write(ToAString(title).c_str());
		log->ChangeColumn();
		write(text);
		if (!isLast)
		{
			log->ChangeRow();
		}
	};

	{
		DXGI_ADAPTER_DESC1 adapterDesc;

		auto hr = adapter->GetDesc1(&adapterDesc);

		std::ostringstream title, card, vendor, device, subSys, revision, videoMemory, systemMemory, sharedSystemMemory;

		title << "デバイス情報 (" << (index + 1) << ")";

		if (SUCCEEDED(hr))
		{
			card << ToUtf8String((const achar*)adapterDesc.Description);
			vendor << adapterDesc.VendorId;
			device << adapterDesc.DeviceId;
			subSys << adapterDesc.SubSysId;
			revision << adapterDesc.Revision;
			videoMemory << (adapterDesc.DedicatedVideoMemory / 1024 / 1024) << "MB";
			systemMemory << (adapterDesc.DedicatedSystemMemory / 1024 / 1024) << "MB";
			sharedSystemMemory << (adapterDesc.SharedSystemMemory / 1024 / 1024) << "MB";
		}

		log->WriteLineStrongly(ToAString(title.str().c_str()).c_str());

		log->BeginTable();

		writeTable("GraphicCard", card, false);
		writeTable("VendorID", vendor, false);
		writeTable("DeviceID", device, false);
		writeTable("SubSysID", subSys, false);
		writeTable("Revision", revision, false);
		writeTable("VideoMemory", videoMemory, false);
		writeTable("SystemMemory", systemMemory, false);
		writeTable("SharedSystemMemory", sharedSystemMemory, true);

		log->EndTable();
	}

	for (int32_t i = 0;; i++)
	{
		IDXGIOutput* temp = nullptr;
		if (adapter->EnumOutputs(i, &temp) != DXGI_ERROR_NOT_FOUND)
		{
			DXGI_OUTPUT_DESC outputDesc;

			if (SUCCEEDED(temp->GetDesc(&outputDesc)))
			{
				std::ostringstream title, name, attach, pos;

				title << "アウトプット情報 (" << (i + 1) << ")";

				name << ToUtf8String((const achar*)outputDesc.DeviceName);
				attach << (outputDesc.AttachedToDesktop == TRUE ? "True" : "False");
				pos << "(" << outputDesc.DesktopCoordinates.left << ","
					<< outputDesc.DesktopCoordinates.top << ","
					<< (outputDesc.DesktopCoordinates.right - outputDesc.DesktopCoordinates.left) << ","
					<< (outputDesc.DesktopCoordinates.bottom - outputDesc.DesktopCoordinates.top) << ")";
					
				log->WriteLineStrongly(ToAString(title.str().c_str()).c_str());
				log->BeginTable();
				writeTable("Name", name, false);
				writeTable("AttachedToDesktop", attach, false);
				writeTable("Coordinate", pos, true);
				log->EndTable();
			}

			SafeRelease(temp);
		}
		else
		{
			break;
		}
	}
}

void Graphics_Imp_DX11::ApplyRenderTargets()
{
	if (isRenderTargetDirty)
	{
		// reset all
		for (auto& r : renderTargets)
		{
			SafeAddRef(r);
		}
		SafeAddRef(depthTarget);

		for (auto& r : currentRenderTargets)
		{
			SafeRelease(r);
		}
		SafeRelease(currentDepthTarget);

		// end scene
		if (isSceneRunning)
		{
			rhiContext->End();
			GetRHI()->EndScene();
		}

		// set parameter
		currentRenderTargets = renderTargets;
		currentDepthTarget = depthTarget;

		ar::SceneParameter sceneParam;
		
		for (int32_t i = 0; i < 4; i++)
		{
			if (currentRenderTargets[i] == nullptr) continue;
			auto rt = (RenderTexture2D_Imp_DX11*)currentRenderTargets[i];
			
			sceneParam.RenderTargets[i] = rt->GetRHI();
		}

		if (currentDepthTarget != nullptr)
		{
			auto db = (DepthBuffer_Imp_DX11*)currentDepthTarget;
			sceneParam.DepthTarget = db->GetRHI();
		}

		GetRHI()->BeginScene(sceneParam);
		rhiContext->Begin();

		isRenderTargetDirty = false;
		isSceneRunning = true;
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
VertexBuffer_Imp* Graphics_Imp_DX11::CreateVertexBuffer_Imp_(int32_t size, int32_t count, bool isDynamic)
{
	return VertexBuffer_Imp_DX11::Create(this, size, count, isDynamic);
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
IndexBuffer_Imp* Graphics_Imp_DX11::CreateIndexBuffer_Imp_(int maxCount, bool isDynamic, bool is32bit)
{
	return IndexBuffer_Imp_DX11::Create(this, maxCount, isDynamic, is32bit);
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
NativeShader_Imp* Graphics_Imp_DX11::CreateShader_Imp_(
	const char* vertexShaderText,
	const char* vertexShaderFileName,
	const char* pixelShaderText,
	const char* pixelShaderFileName,
	std::vector <VertexLayout>& layout,
	bool is32Bit,
	std::vector <Macro>& macro)
{
	return NativeShader_Imp_DX11::Create(
		this,
		vertexShaderText,
		vertexShaderFileName,
		pixelShaderText,
		pixelShaderFileName,
		layout,
		is32Bit,
		macro,
		m_log);
}

NativeShader_Imp* Graphics_Imp_DX11::CreateShader_Imp_(
	const uint8_t* vertexShader,
	int32_t vertexShaderSize,
	const uint8_t* pixelShader,
	int32_t pixelShaderSize,
	std::vector <VertexLayout>& layout,
	bool is32Bit)
{
	return NativeShader_Imp_DX11::Create(
		this,
		vertexShader,
		vertexShaderSize,
		pixelShader,
		pixelShaderSize,
		layout,
		is32Bit);
}
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Graphics_Imp_DX11::UpdateDrawStates(VertexBuffer_Imp* vertexBuffer, IndexBuffer_Imp* indexBuffer, NativeShader_Imp* shaderPtr, int32_t& vertexBufferOffset)
{
	assert(vertexBuffer != nullptr);
	assert(indexBuffer != nullptr);
	assert(shaderPtr != nullptr);

	auto v_impl = (VertexBuffer_Imp_DX11*)vertexBuffer;
	auto i_impl = (IndexBuffer_Imp_DX11*)indexBuffer;
	auto s_impl = (NativeShader_Imp_DX11*)shaderPtr;

	drawParam.VertexBufferPtr = v_impl->GetRHI();
	drawParam.IndexBufferPtr = i_impl->GetRHI();
	drawParam.ShaderPtr = s_impl->GetRHI();
	drawParam.VertexConstantBufferPtr = s_impl->GetRHIVertexConstantBuffer();
	drawParam.PixelConstantBufferPtr = s_impl->GetRHIPixelConstantBuffer();
	drawParam.VertexShaderTextures.fill(nullptr);
	drawParam.PixelShaderTextures.fill(nullptr);

	vertexBufferOffset = v_impl->GetRHI()->GetVertexOffset();

	s_impl->AssignConstantBuffer();

	for (auto& bt : s_impl->GetBindingTextures())
	{
		if (bt.second.TexturePtr == nullptr) continue;
		auto tex = bt.second.TexturePtr.get();
		auto id = bt.first;

		ar::Texture* rhi = nullptr;

		if (bt.second.TexturePtr->GetType() == TextureClassType::Texture2D)
		{
			auto t = (Texture2D_Imp_DX11*)tex;
			rhi = t->GetRHI();
		}
		else if (tex->GetType() == TextureClassType::RenderTexture2D)
		{
			auto t = (RenderTexture2D_Imp_DX11*)tex;
			rhi = t->GetRHI();
		}
		else if (tex->GetType() == TextureClassType::CubemapTexture)
		{
			auto t = (CubemapTexture_Imp_DX11*)tex;
			rhi = t->GetRHI();
		}
		else if (tex->GetType() == TextureClassType::DepthBuffer)
		{
			auto t = (DepthBuffer_Imp_DX11*)tex;
			rhi = t->GetRHI();
		}

		if (id >= 0xff)
		{
			// 頂点シェーダーに設定
			if (id - 0xff < drawParam.VertexShaderTextures.size())
			{
				drawParam.VertexShaderTextures[id - 0xff] = rhi;
			}

			nextState.textureFilterTypes_vs[id - 0xff] = bt.second.FilterType;
			nextState.textureWrapTypes_vs[id - 0xff] = bt.second.WrapType;
		}
		else
		{
			// ピクセルシェーダーに設定
			if (id < drawParam.PixelShaderTextures.size())
			{
				drawParam.PixelShaderTextures[id] = rhi;
			}

			// ステート設定
			nextState.textureFilterTypes[id] = bt.second.FilterType;
			nextState.textureWrapTypes[id] = bt.second.WrapType;
		}
	}

	CommitRenderState(false);
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Graphics_Imp_DX11::DrawPolygonInternal(int32_t count, VertexBuffer_Imp* vertexBuffer, IndexBuffer_Imp* indexBuffer, NativeShader_Imp* shaderPtr)
{
	int32_t vertexBufferOffset = 0;

	UpdateDrawStates(vertexBuffer, indexBuffer, shaderPtr, vertexBufferOffset);
	
	drawParam.InstanceCount = 1;
	drawParam.VertexOffset = vertexBufferOffset;
	drawParam.IndexCount = count * 3;
	drawParam.IndexOffset = 0;

	ApplyRenderTargets();

	rhiContext->Draw(drawParam);
}

void Graphics_Imp_DX11::DrawPolygonInternal(int32_t offset, int32_t count, VertexBuffer_Imp* vertexBuffer, IndexBuffer_Imp* indexBuffer, NativeShader_Imp* shaderPtr)
{
	int32_t vertexBufferOffset = 0;

	UpdateDrawStates(vertexBuffer, indexBuffer, shaderPtr, vertexBufferOffset);

	drawParam.InstanceCount = 1;
	drawParam.VertexOffset = vertexBufferOffset;
	drawParam.IndexCount = count * 3;
	drawParam.IndexOffset = offset * 3;

	ApplyRenderTargets();

	rhiContext->Draw(drawParam);
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Graphics_Imp_DX11::DrawPolygonInstancedInternal(int32_t count, VertexBuffer_Imp* vertexBuffer, IndexBuffer_Imp* indexBuffer, NativeShader_Imp* shaderPtr, int32_t instanceCount)
{
	int32_t vertexBufferOffset = 0;

	UpdateDrawStates(vertexBuffer, indexBuffer, shaderPtr, vertexBufferOffset);

	drawParam.InstanceCount = instanceCount;
	drawParam.VertexOffset = 0;
	drawParam.IndexCount = count * 3;
	drawParam.IndexOffset = 0;

	ApplyRenderTargets();

	rhiContext->Draw(drawParam);
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Graphics_Imp_DX11::BeginInternal()
{
	// Reset targets
	for (auto& r : renderTargets)
	{
		SafeRelease(r);
	}
	SafeRelease(depthTarget);

	isRenderTargetDirty = true;

	GetRHI()->BeginRendering();
}

void Graphics_Imp_DX11::EndInternal()
{
	if (isSceneRunning)
	{
		rhiContext->End();
		GetRHI()->EndScene();
		isSceneRunning = false;
	}

	GetRHI()->EndRendering();
}

Graphics_Imp_DX11* Graphics_Imp_DX11::Create(Window* window, void* handle, int32_t width, int32_t height, Log* log, File* file, GraphicsOption option)
{
	auto writeLogHeading = [log](const astring s) -> void
	{
		if (log == nullptr) return;
		log->WriteHeading(s.c_str());
	};

	auto writeLog = [log](const astring s) -> void
	{
		if (log == nullptr) return;
		log->WriteLine(s.c_str());
	};

	if (option.GraphicsDevice == GraphicsDeviceType::DirectX11)
	{
		writeLogHeading(ToAString("描画(DirectX11)"));
	}
	else if (option.GraphicsDevice == GraphicsDeviceType::OpenGL)
	{
		writeLogHeading(ToAString("描画(OpenGL)"));
	}

	// ShowData
	if(option.GraphicsDevice == GraphicsDeviceType::DirectX11)
	{
		IDXGIFactory1* dxgiFactory = NULL;
		std::vector<IDXGIAdapter1*> adapters;

		HRESULT hr;

		hr = CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)&dxgiFactory);
		if (dxgiFactory == NULL)
		{
			writeLog(ToAString("ファクトリの作成に失敗"));
			goto End;
		}

		for (int32_t i = 0;; i++)
		{
			IDXGIAdapter1* temp = 0;
			if (dxgiFactory->EnumAdapters1(i, &temp) != DXGI_ERROR_NOT_FOUND)
			{
				WriteAdapterInformation(log, temp, i);
				temp->Release();
			}
			else
			{
				break;
			}
		}

		dxgiFactory->Release();
	}

	auto window_ = (Window_Imp*)window;
	window_->GetWindow()->MakeContextCurrent();

	ar::Manager* manager = ar::Manager::Create((ar::GraphicsDeviceType)option.GraphicsDevice);
	ar::ManagerInitializationParameter initParam;
	initParam.WindowWidth = width;
	initParam.WindowHeight = height;
	initParam.IsFullscreenMode = option.IsFullScreen;
	initParam.ColorSpace = (ar::ColorSpaceType)option.ColorSpace;
	initParam.Handles[0] = handle;

	auto errorCode = manager->Initialize(initParam);

	if (errorCode == ar::ErrorCode::OK)
	{

	}
	else if (errorCode == ar::ErrorCode::FailedToCreateFactory)
	{
		writeLog(ToAString("ファクトリの作成に失敗"));
		goto End;
	}
	else if (errorCode == ar::ErrorCode::FailedToGetAdapter)
	{
		writeLog(ToAString("アダプタの取得に失敗"));
		goto End;
	}
	else if (errorCode == ar::ErrorCode::FailedToCreateDevice)
	{
		writeLog(ToAString("デバイスの作成に失敗"));
		goto End;
	}
	else if (errorCode == ar::ErrorCode::FailedToCreateSwapChain)
	{
		writeLog(ToAString("スワップチェーンの作成に失敗"));
		goto End;
	}
	else if (errorCode == ar::ErrorCode::FailedToGetBackBuffer)
	{
		writeLog(ToAString("バックバッファの取得に失敗"));
		goto End;
	}
	else if (errorCode == ar::ErrorCode::FailedToCreateBackBufferTarget)
	{
		writeLog(ToAString("バックバッファのレンダーターゲットの取得に失敗"));
		goto End;
	}
	else if (errorCode == ar::ErrorCode::FailedToInitializeGlew)
	{
		writeLog(ToAString("Glewの初期化に失敗"));
		goto End;
	}

return new Graphics_Imp_DX11(
	manager,
	window,
	Vector2DI(width, height),
	log,
	file,
	option);
End:

	asd::SafeDelete(manager);

	writeLog(ToAString("DirectX11初期化失敗"));
	writeLog(ToAString(""));

	return nullptr;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Graphics_Imp_DX11* Graphics_Imp_DX11::Create(Window* window, Log* log, File* file, GraphicsOption option)
{
	auto size = window->GetSize();
	auto handle = ((Window_Imp*)window)->GetWindowHandle();
	return Create(window, handle, size.X, size.Y, log, file, option);
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Graphics_Imp_DX11* Graphics_Imp_DX11::Create(void* handle, int32_t width, int32_t height, Log* log, File* file, GraphicsOption option)
{
	return Create(nullptr, handle, width, height, log,file, option);
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Texture2D_Imp* Graphics_Imp_DX11::CreateTexture2D_Imp_Internal(Graphics* graphics, uint8_t* data, int32_t size)
{
	auto ret = Texture2D_Imp_DX11::Create(this, data, size, false, this->GetOption().ColorSpace == ColorSpaceType::LinearSpace);
	return ret;
}

Texture2D_Imp* Graphics_Imp_DX11::CreateTexture2DAsRawData_Imp_Internal(Graphics* graphics, uint8_t* data, int32_t size)
{
	auto ret = Texture2D_Imp_DX11::Create(this, data, size, false, false);
	return ret;
}

Texture2D_Imp* Graphics_Imp_DX11::CreateEmptyTexture2D_Imp_Internal(Graphics* graphics, int32_t width, int32_t height, TextureFormat format, void* data)
{
	auto ret = Texture2D_Imp_DX11::Create(this, width, height, format, data);
	return ret;
}

Texture2D_Imp* Graphics_Imp_DX11::CreateEditableTexture2D_Imp_Internal(Graphics* graphics, uint8_t* data, int32_t size)
{
	auto ret = Texture2D_Imp_DX11::Create(this, data, size, true, true);
	return ret;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
RenderTexture2D_Imp* Graphics_Imp_DX11::CreateRenderTexture2D_Imp(int32_t width, int32_t height, TextureFormat format)
{
	return RenderTexture2D_Imp_DX11::Create(this, width, height, format);
}

CubemapTexture* Graphics_Imp_DX11::CreateCubemapTextureFrom6ImageFiles_(const achar* front, const achar* left, const achar* back, const achar* right, const achar* top, const achar* bottom)
{
	return CubemapTexture_Imp_DX11::Create(this, front, left, back, right, top, bottom);
}

CubemapTexture* Graphics_Imp_DX11::CreateCubemapTextureFromMipmapImageFiles_(const achar* path, int32_t mipmapCount)
{
	return CubemapTexture_Imp_DX11::Create(this, path, mipmapCount);
}

CubemapTexture* Graphics_Imp_DX11::CreateCubemapTextureFromSingleImageFile_(const achar* path)
{
	return CubemapTexture_Imp_DX11::Create(this, path);
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
DepthBuffer_Imp* Graphics_Imp_DX11::CreateDepthBuffer_Imp(int32_t width, int32_t height)
{
	return DepthBuffer_Imp_DX11::Create(this, width, height);
}

void Graphics_Imp_DX11::CommitRenderState(bool forced)
{
	currentState = nextState;

	drawParam.AlphaBlend = (ar::AlphaBlendMode)currentState.renderState.AlphaBlendState;
	drawParam.Culling = (ar::CullingType)currentState.renderState.Culling;
	drawParam.IsDepthTest = currentState.renderState.DepthTest;
	drawParam.IsDepthWrite = currentState.renderState.DepthWrite;

	for (int32_t i = 0; i < drawParam.VertexShaderTextures.size(); i++)
	{
		drawParam.VertexShaderTextureFilers[i] = (ar::TextureFilterType)currentState.textureFilterTypes_vs[i];
		drawParam.VertexShaderTextureWraps[i] = (ar::TextureWrapType)currentState.textureWrapTypes_vs[i];
	}
	
	for (int32_t i = 0; i < drawParam.PixelShaderTextures.size(); i++)
	{
		drawParam.PixelShaderTextureFilers[i] = (ar::TextureFilterType)currentState.textureFilterTypes[i];
		drawParam.PixelShaderTextureWraps[i] = (ar::TextureWrapType)currentState.textureWrapTypes[i];
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Graphics_Imp_DX11::SetRenderTarget(RenderTexture2D_Imp* texture, DepthBuffer_Imp* depthBuffer)
{
	SafeAddRef(texture);
	SafeAddRef(depthBuffer);

	for (auto& r : renderTargets)
	{
		SafeRelease(r);
	}
	SafeRelease(depthTarget);

	if (texture == currentRenderTargets[0] &&
		nullptr == currentRenderTargets[1] &&
		nullptr == currentRenderTargets[2] &&
		nullptr == currentRenderTargets[3] &&
		depthBuffer == currentDepthTarget)
	{
		isRenderTargetDirty = false;
	}
	else
	{
		isRenderTargetDirty = true;
	}

	renderTargets[0] = texture;
	depthTarget = depthBuffer;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Graphics_Imp_DX11::SetRenderTarget(RenderTexture2D_Imp* texture1, RenderTexture2D_Imp* texture2, RenderTexture2D_Imp* texture3, RenderTexture2D_Imp* texture4, DepthBuffer_Imp* depthBuffer)
{
	SafeAddRef(texture1);
	SafeAddRef(texture2);
	SafeAddRef(texture3);
	SafeAddRef(texture4);
	SafeAddRef(depthBuffer);

	for (auto& r : renderTargets)
	{
		SafeRelease(r);
	}
	SafeRelease(depthTarget);

	isRenderTargetDirty = true;

	renderTargets[0] = texture1;
	renderTargets[1] = texture2;
	renderTargets[2] = texture3;
	renderTargets[3] = texture4;
	depthTarget = depthBuffer;
}

void Graphics_Imp_DX11::SetRenderTarget(CubemapTexture_Imp* texture, int32_t direction, int32_t mipmap, DepthBuffer_Imp* depthBuffer)
{
	assert(0);

	/*
	auto tex = (CubemapTexture_Imp_DX11*) texture;

	// 強制リセット(テクスチャと描画先同時設定不可のため)
	for (int32_t i = 0; i < Graphics_Imp::MaxTextureCount; i++)
	{
		ID3D11ShaderResourceView* rv = { nullptr };
		GetContext()->VSSetShaderResources(i, 1, &rv);
		GetContext()->PSSetShaderResources(i, 1, &rv);
	}

	if (texture == nullptr)
	{
		m_context->OMSetRenderTargets(1, &m_defaultBackRenderTargetView, m_defaultDepthStencilView);
		SetViewport(0, 0, m_size.X, m_size.Y);

		for (auto i = 0; i < MaxRenderTarget; i++)
		{
			SafeRelease(m_currentBackRenderTargetViews[i]);
		}
		m_currentBackRenderTargetViews[0] = m_defaultBackRenderTargetView;
		SafeAddRef(m_currentBackRenderTargetViews[0]);

		SafeRelease(m_currentDepthStencilView);
		m_currentDepthStencilView = m_defaultDepthStencilView;
		SafeAddRef(m_currentDepthStencilView);

		return;
	}

	ID3D11RenderTargetView* rt = nullptr;
	ID3D11DepthStencilView* ds = nullptr;

	if (texture != nullptr)
	{
		rt = tex->GetRenderTargetView(direction, mipmap);
	}

	if (depthBuffer != nullptr)
	{
		ds = ((DepthBuffer_Imp_DX11*) depthBuffer)->GetDepthStencilView();
	}

	if (rt != nullptr)
	{
		m_context->OMSetRenderTargets(1, &rt, ds);
		SetViewport(0, 0, tex->GetSize().X >> mipmap, tex->GetSize().Y >> mipmap);

		for (auto i = 0; i < MaxRenderTarget; i++)
		{
			SafeRelease(m_currentBackRenderTargetViews[i]);
		}
		m_currentBackRenderTargetViews[0] = rt;
		SafeAddRef(m_currentBackRenderTargetViews[0]);

		SafeRelease(m_currentDepthStencilView);
		m_currentDepthStencilView = ds;
		SafeAddRef(m_currentDepthStencilView);
	}
	*/
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Graphics_Imp_DX11::MakeContextCurrent()
{
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Graphics_Imp_DX11::FlushCommand()
{
}

void Graphics_Imp_DX11::SetIsFullscreenMode(bool isFullscreenMode)
{
	GetRHI()->SetIsFullscreenMode(isFullscreenMode);
}

void Graphics_Imp_DX11::SetWindowSize(Vector2DI size)
{
	GetRHI()->SetWindowSize(size.X, size.Y);
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Graphics_Imp_DX11::Clear(bool isColorTarget, bool isDepthTarget, const Color& color)
{
	ApplyRenderTargets();

	ar::Color color_;
	color_.R = color.R;
	color_.G = color.G;
	color_.B = color.B;
	color_.A = color.A;

	GetRHI()->Clear(isColorTarget, isDepthTarget, color_);
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Graphics_Imp_DX11::Present()
{
	GetRHI()->Present();

	if (m_window != nullptr)
	{
		auto window_ = (Window_Imp*)m_window;
		window_->GetWindow()->Present();
	}
}

void Graphics_Imp_DX11::SaveScreenshot(const achar* path)
{
	std::vector<Color> bufs;
	Vector2DI size;

	SaveScreenshot(bufs, size);

	ImageHelper::SaveImage(path, size.X, size.Y, bufs.data(), false);
}

void Graphics_Imp_DX11::SaveScreenshot(std::vector<Color>& bufs, Vector2DI& size)
{
	std::vector<ar::Color> dst;
	int32_t width;
	int32_t height;

	GetRHI()->SaveScreen(dst, width, height);

	bufs.resize(width * height);
	memcpy(bufs.data(), dst.data(), width * height * sizeof(Color));
}

}
