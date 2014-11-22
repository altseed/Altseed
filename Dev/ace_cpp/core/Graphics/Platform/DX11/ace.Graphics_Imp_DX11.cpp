
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "ace.Graphics_Imp_DX11.h"
#include "../../ace.RenderingThread.h"

#include "../../../Log/ace.Log.h"
#include "../../../Window/ace.Window_Imp.h"
#include "Resource/ace.VertexBuffer_Imp_DX11.h"
#include "Resource/ace.IndexBuffer_Imp_DX11.h"
#include "Resource/ace.NativeShader_Imp_DX11.h"
#include "Resource/ace.DepthBuffer_Imp_DX11.h"

#include "Resource/ace.Texture2D_Imp_DX11.h"
#include "Resource/ace.RenderTexture2D_Imp_DX11.h"
#include "Resource/ace.CubemapTexture_Imp_DX11.h"

#include <sstream>

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {
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
Graphics_Imp_DX11::Graphics_Imp_DX11(
	Window* window,
	Vector2DI size,
	Log* log,
	bool isReloadingEnabled,
	bool isFullScreen,
	ID3D11Device* device,
	ID3D11DeviceContext* context,
	IDXGIDevice1* dxgiDevice,
	IDXGIAdapter1* adapter,
	IDXGIFactory1* dxgiFactory,
	IDXGISwapChain* swapChain,
	ID3D11Texture2D* defaultBack,
	ID3D11RenderTargetView*	defaultBackRenderTargetView,
	ID3D11Texture2D* defaultDepthBuffer,
	ID3D11DepthStencilView* defaultDepthStencilView)
	: Graphics_Imp(size, log, isReloadingEnabled, isFullScreen)
	, m_window(window)
	, m_device(device)
	, m_context(context)
	, m_dxgiDevice(dxgiDevice)
	, m_adapter(adapter)
	, m_dxgiFactory(dxgiFactory)
	, m_swapChain(swapChain)
	, m_defaultBack(defaultBack)
	, m_defaultBackRenderTargetView(defaultBackRenderTargetView)
	, m_defaultDepthBuffer(defaultDepthBuffer)
	, m_defaultDepthStencilView(defaultDepthStencilView)
	, m_currentDepthStencilView(nullptr)
{
	SafeAddRef(window);

	for (auto i = 0; i < MaxRenderTarget; i++)
	{
		m_currentBackRenderTargetViews[i] = nullptr;
	}

	GenerateRenderStates();

	m_renderingThread->Run(this, StartRenderingThreadFunc, EndRenderingThreadFunc);
	
	GetEffectSetting()->SetTextureLoader(new EffectTextureLoader_DX11(this));
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Graphics_Imp_DX11::~Graphics_Imp_DX11()
{
	m_renderingThread->AddEvent(nullptr);
	while (m_renderingThread->IsRunning())
	{
		Sleep(1);
	}
	m_renderingThread.reset();
	
	for (auto i = 0; i < MaxRenderTarget; i++)
	{
		SafeRelease(m_currentBackRenderTargetViews[i]);
	}

#pragma region RenderStates
	for (int32_t ct = 0; ct < CulTypeCount; ct++)
	{
		SafeRelease(m_rStates[ct]);
	}

	for (int32_t dt = 0; dt < DepthTestCount; dt++)
	{
		for (int32_t dw = 0; dw < DepthWriteCount; dw++)
		{
			SafeRelease(m_dStates[dt][dw]);
		}
	}

	for (int32_t i = 0; i < AlphaTypeCount; i++)
	{
		SafeRelease(m_bStates[i]);
	}

	for (int32_t f = 0; f < TextureFilterCount; f++)
	{
		for (int32_t w = 0; w < TextureWrapCount; w++)
		{
			SafeRelease(m_sStates[f][w]);
		}
	}
#pragma endregion


	SafeRelease(m_currentDepthStencilView);

	SafeRelease(m_defaultBack);
	SafeRelease(m_defaultBackRenderTargetView);

	SafeRelease(m_defaultDepthBuffer);
	SafeRelease(m_defaultDepthStencilView);

	SafeRelease(m_device);
	SafeRelease(m_context);
	SafeRelease(m_dxgiDevice);
	SafeRelease(m_adapter);
	SafeRelease(m_dxgiFactory);
	SafeRelease(m_swapChain);

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
			card << ToUtf8String(adapterDesc.Description);
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

				name << ToUtf8String(outputDesc.DeviceName);
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

void Graphics_Imp_DX11::GenerateRenderStates()
{
	D3D11_CULL_MODE cullTbl [] =
	{
		D3D11_CULL_BACK,
		D3D11_CULL_FRONT,
		D3D11_CULL_NONE,
	};

	for (int32_t ct = 0; ct < CulTypeCount; ct++)
	{
		D3D11_RASTERIZER_DESC rsDesc;
		ZeroMemory(&rsDesc, sizeof(D3D11_RASTERIZER_DESC));
		rsDesc.CullMode = cullTbl[ct];
		rsDesc.FillMode = D3D11_FILL_SOLID;
		rsDesc.DepthClipEnable = TRUE;
		GetDevice()->CreateRasterizerState(&rsDesc, &m_rStates[ct]);
	}

	for (int32_t dt = 0; dt < DepthTestCount; dt++)
	{
		for (int32_t dw = 0; dw < DepthWriteCount; dw++)
		{
			D3D11_DEPTH_STENCIL_DESC dsDesc;
			ZeroMemory(&dsDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
			dsDesc.DepthEnable = dt;
			dsDesc.DepthWriteMask = (D3D11_DEPTH_WRITE_MASK) dw;
			dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
			dsDesc.StencilEnable = FALSE;
			GetDevice()->CreateDepthStencilState(&dsDesc, &m_dStates[dt][dw]);
		}
	}

	for (int32_t i = 0; i < AlphaTypeCount; i++)
	{
		D3D11_BLEND_DESC Desc;
		ZeroMemory(&Desc, sizeof(Desc));
		Desc.AlphaToCoverageEnable = false;

		for (int32_t k = 0; k < 8; k++)
		{
			Desc.RenderTarget[k].BlendEnable = i != (int32_t) AlphaBlend::Opacity;
			Desc.RenderTarget[k].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
			Desc.RenderTarget[k].SrcBlendAlpha = D3D11_BLEND_ONE;
			Desc.RenderTarget[k].DestBlendAlpha = D3D11_BLEND_ONE;
			Desc.RenderTarget[k].BlendOpAlpha = D3D11_BLEND_OP_MAX;

			switch (i)
			{
			case (int32_t) AlphaBlend::Opacity:
				Desc.RenderTarget[k].DestBlend = D3D11_BLEND_ZERO;
				Desc.RenderTarget[k].SrcBlend = D3D11_BLEND_ONE;
				Desc.RenderTarget[k].BlendOp = D3D11_BLEND_OP_ADD;
				break;
			case (int32_t) AlphaBlend::Blend:
				Desc.RenderTarget[k].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
				Desc.RenderTarget[k].SrcBlend = D3D11_BLEND_SRC_ALPHA;
				Desc.RenderTarget[k].BlendOp = D3D11_BLEND_OP_ADD;
				break;
			case (int32_t) AlphaBlend::Add:
				Desc.RenderTarget[k].DestBlend = D3D11_BLEND_ONE;
				Desc.RenderTarget[k].SrcBlend = D3D11_BLEND_SRC_ALPHA;
				Desc.RenderTarget[k].BlendOp = D3D11_BLEND_OP_ADD;
				break;
			case (int32_t) AlphaBlend::Sub:
				Desc.RenderTarget[k].DestBlend = D3D11_BLEND_ONE;
				Desc.RenderTarget[k].SrcBlend = D3D11_BLEND_SRC_ALPHA;
				Desc.RenderTarget[k].BlendOp = D3D11_BLEND_OP_REV_SUBTRACT;
				break;

			case (int32_t) AlphaBlend::Mul:
				Desc.RenderTarget[k].DestBlend = D3D11_BLEND_SRC_COLOR;
				Desc.RenderTarget[k].SrcBlend = D3D11_BLEND_ZERO;
				Desc.RenderTarget[k].BlendOp = D3D11_BLEND_OP_ADD;
				break;
			case (int32_t) AlphaBlend::AddAll:
				Desc.RenderTarget[k].DestBlend = D3D11_BLEND_ONE;
				Desc.RenderTarget[k].SrcBlend = D3D11_BLEND_ONE;
				Desc.RenderTarget[k].BlendOp = D3D11_BLEND_OP_ADD;
				break;
			}
		}

		GetDevice()->CreateBlendState(&Desc, &m_bStates[i]);
	}

	for (int32_t f = 0; f < TextureFilterCount; f++)
	{
		for (int32_t w = 0; w < TextureWrapCount; w++)
		{
			D3D11_TEXTURE_ADDRESS_MODE Addres [] = {
				D3D11_TEXTURE_ADDRESS_WRAP,
				D3D11_TEXTURE_ADDRESS_CLAMP,
			};

			D3D11_FILTER Filter [] = {
				D3D11_FILTER_MIN_MAG_MIP_POINT,
				D3D11_FILTER_MIN_MAG_MIP_LINEAR,
			};

			uint32_t Anisotropic [] = {
				0, 0,
			};

			D3D11_SAMPLER_DESC SamlerDesc = {
				Filter[f],
				Addres[w],
				Addres[w],
				Addres[w],
				0.0f,
				Anisotropic[f],
				D3D11_COMPARISON_ALWAYS,
				{ 0.0f, 0.0f, 0.0f, 0.0f },
				0.0f,
				D3D11_FLOAT32_MAX, };

			GetDevice()->CreateSamplerState(&SamlerDesc, &m_sStates[f][w]);
		}
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
	std::vector <Macro>& macro)
{
	return NativeShader_Imp_DX11::Create(
		this,
		vertexShaderText,
		vertexShaderFileName,
		pixelShaderText,
		pixelShaderFileName,
		layout,
		macro,
		m_log);
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Graphics_Imp_DX11::UpdateDrawStates(VertexBuffer_Imp* vertexBuffer, IndexBuffer_Imp* indexBuffer, NativeShader_Imp* shaderPtr, int32_t& vertexBufferOffset)
{
	assert(vertexBuffer != nullptr);
	assert(indexBuffer != nullptr);
	assert(shaderPtr != nullptr);

	{
		auto vBuf = ((VertexBuffer_Imp_DX11*)vertexBuffer)->GetBuffer();
		uint32_t vertexSize = vertexBuffer->GetSize();
		uint32_t offset = 0;
		GetContext()->IASetVertexBuffers(0, 1, &vBuf, &vertexSize, &offset);
		vertexBufferOffset = vertexBuffer->GetVertexOffset();
	}
	
	{
		auto buf = ((IndexBuffer_Imp_DX11*) indexBuffer)->GetBuffer();
		if (indexBuffer->Is32Bit())
		{
			GetContext()->IASetIndexBuffer(buf, DXGI_FORMAT_R32_UINT, 0);
		}
		else
		{
			GetContext()->IASetIndexBuffer(buf, DXGI_FORMAT_R16_UINT, 0);
		}
	}

	{
		auto shader = (NativeShader_Imp_DX11*) shaderPtr;

		// シェーダーの設定
		GetContext()->VSSetShader(shader->GetVertexShader(), NULL, 0);
		GetContext()->PSSetShader(shader->GetPixelShader(), NULL, 0);

		// レイアウトの設定
		GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		GetContext()->IASetInputLayout(shader->GetLayout());

		// 定数バッファの割り当て
		shaderPtr->AssignConstantBuffer();

		// テクスチャの設定
		for (auto& bt : shader->GetBindingTextures())
		{
			if (bt.second.TexturePtr == nullptr) continue;
			auto tex = bt.second.TexturePtr.get();
			auto id = bt.first;

			ID3D11ShaderResourceView* rv = nullptr;

			if (bt.second.TexturePtr->GetType() == TEXTURE_CLASS_TEXTURE2D)
			{
				auto t = (Texture2D_Imp_DX11*) tex;
				rv = t->GetShaderResourceView();
			}
			else if (tex->GetType() == TEXTURE_CLASS_RENDERTEXTURE2D)
			{
				auto t = (RenderTexture2D_Imp_DX11*) tex;
				rv = t->GetShaderResourceView();
			}
			else if (tex->GetType() == TEXTURE_CLASS_CUBEMAPTEXTURE)
			{
				auto t = (CubemapTexture_Imp_DX11*) tex;
				rv = t->GetShaderResourceView();
			}
			else if (tex->GetType() == TEXTURE_CLASS_DEPTHBUFFER)
			{
				auto t = (DepthBuffer_Imp_DX11*) tex;
				rv = t->GetShaderResourceView();
			}

			if (id >= 0xff)
			{
				// 頂点シェーダーに設定
				GetContext()->VSSetShaderResources(id - 0xff, 1, &rv);

				nextState.textureFilterTypes_vs[id - 0xff] = bt.second.FilterType;
				nextState.textureWrapTypes_vs[id - 0xff] = bt.second.WrapType;
			}
			else
			{
				// ピクセルシェーダーに設定
				GetContext()->PSSetShaderResources(id, 1, &rv);

				// ステート設定
				nextState.textureFilterTypes[id] = bt.second.FilterType;
				nextState.textureWrapTypes[id] = bt.second.WrapType;
			}
		}

		/*
		for (int32_t i = 0; i < Graphics_Imp::MaxTextureCount; i++)
		{
			Texture* tex = nullptr;
			char* texName = nullptr;
			TextureFilterType filterType;
			TextureWrapType wrapType;

			if (shader->GetTexture(texName, tex, filterType, wrapType, i))
			{
				ID3D11ShaderResourceView* rv = nullptr;

				if (tex->GetType() == TEXTURE_CLASS_TEXTURE2D)
				{
					auto t = (Texture2D_Imp_DX11*) tex;
					rv = t->GetShaderResourceView();
				}
				else if (tex->GetType() == TEXTURE_CLASS_RENDERTEXTURE2D)
				{
					auto t = (RenderTexture2D_Imp_DX11*) tex;
					rv = t->GetShaderResourceView();
				}
				else if (tex->GetType() == TEXTURE_CLASS_CUBEMAPTEXTURE)
				{
					auto t = (CubemapTexture_Imp_DX11*) tex;
					rv = t->GetShaderResourceView();
				}
				else if (tex->GetType() == TEXTURE_CLASS_DEPTHBUFFER)
				{
					auto t = (DepthBuffer_Imp_DX11*) tex;
					rv = t->GetShaderResourceView();
				}

				// 頂点シェーダーに設定
				GetContext()->VSSetShaderResources(i, 1, &rv);

				// ピクセルシェーダーに設定
				GetContext()->PSSetShaderResources(i, 1, &rv);

				// ステート設定
				nextState.textureFilterTypes[i] = filterType;
				nextState.textureWrapTypes[i] = wrapType;
			}
		}
		*/
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
	GetContext()->DrawIndexed(
		count * 3,
		0,
		vertexBufferOffset);
}

void Graphics_Imp_DX11::DrawPolygonInternal(int32_t offset, int32_t count, VertexBuffer_Imp* vertexBuffer, IndexBuffer_Imp* indexBuffer, NativeShader_Imp* shaderPtr)
{
	int32_t vertexBufferOffset = 0;

	UpdateDrawStates(vertexBuffer, indexBuffer, shaderPtr, vertexBufferOffset);
	GetContext()->DrawIndexed(
		count * 3,
		offset * 3,
		vertexBufferOffset);
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Graphics_Imp_DX11::DrawPolygonInstancedInternal(int32_t count, VertexBuffer_Imp* vertexBuffer, IndexBuffer_Imp* indexBuffer, NativeShader_Imp* shaderPtr, int32_t instanceCount)
{
	int32_t vertexBufferOffset = 0;

	UpdateDrawStates(vertexBuffer, indexBuffer, shaderPtr, vertexBufferOffset);
	GetContext()->DrawIndexedInstanced(
		count * 3,
		instanceCount,
		0,
		0,
		0);
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Graphics_Imp_DX11::BeginInternal()
{
	// 描画先のリセット
	m_context->OMSetRenderTargets(1, &m_defaultBackRenderTargetView, m_defaultDepthStencilView);

	for (auto i = 0; i < MaxRenderTarget; i++)
	{
		SafeRelease(m_currentBackRenderTargetViews[i]);
	}

	m_currentBackRenderTargetViews[0] = m_defaultBackRenderTargetView;
	SafeAddRef(m_currentBackRenderTargetViews[0]);

	SafeRelease(m_currentDepthStencilView);
	m_currentDepthStencilView = m_defaultDepthStencilView;
	SafeAddRef(m_currentDepthStencilView);

	// 描画範囲のリセット
	SetViewport(0, 0, m_size.X, m_size.Y);
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Graphics_Imp_DX11* Graphics_Imp_DX11::Create(Window* window, HWND handle, int32_t width, int32_t height, Log* log, bool isReloadingEnabled, bool isFullScreen)
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

	writeLogHeading(ToAString("DirectX11"));

	/* DirectX初期化 */
	ID3D11Device*			device = NULL;
	ID3D11DeviceContext*	context = NULL;
	IDXGIDevice1*			dxgiDevice = NULL;
	IDXGIAdapter1*			adapter = NULL;
	std::vector<IDXGIAdapter1*>	adapters;
	IDXGIFactory1*			dxgiFactory = NULL;
	IDXGISwapChain*			swapChain = NULL;
	ID3D11Texture2D*		defaultBack = NULL;
	ID3D11RenderTargetView*	defaultBackRenderTargetView = NULL;
	ID3D11Texture2D* depthBuffer = nullptr;
	ID3D11DepthStencilView* depthStencilView = nullptr;

	HRESULT hr;

	hr = CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**) &dxgiFactory);
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
			adapters.push_back(temp);
		}
		else
		{
			break;
		}
	}

	if (adapters.size() > 0)
	{
		SafeAddRef(adapters[0]);
		adapter = adapters[0];
	}

	if (adapter == nullptr)
	{
		writeLog(ToAString("アダプタの取得に失敗"));
		goto End;
	}

	UINT debugFlag = 0;

#if _DEBUG
	debugFlag = D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL flevels[] = {
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1,
	};
	int32_t flevelCount = sizeof(flevels) / sizeof(D3D_FEATURE_LEVEL);

	D3D_FEATURE_LEVEL currentFeatureLevel;

	hr = D3D11CreateDevice(
		adapter,
		D3D_DRIVER_TYPE_UNKNOWN,
		NULL,
		debugFlag,
		flevels,
		flevelCount,
		D3D11_SDK_VERSION,
		&device,
		&currentFeatureLevel,
		&context);

	if FAILED(hr)
	{
		writeLog(ToAString("デバイスの作成に失敗"));
		goto End;
	}

	if (currentFeatureLevel == D3D_FEATURE_LEVEL_11_0) writeLog(ToAString("レベル11.0でデバイスを作成"));
	if (currentFeatureLevel == D3D_FEATURE_LEVEL_10_1) writeLog(ToAString("レベル10.1でデバイスを作成"));
	if (currentFeatureLevel == D3D_FEATURE_LEVEL_10_0) writeLog(ToAString("レベル10.0でデバイスを作成"));
	if (currentFeatureLevel == D3D_FEATURE_LEVEL_9_3) writeLog(ToAString("レベル9.3でデバイスを作成"));
	if (currentFeatureLevel == D3D_FEATURE_LEVEL_9_2) writeLog(ToAString("レベル9.2でデバイスを作成"));
	if (currentFeatureLevel == D3D_FEATURE_LEVEL_9_1) writeLog(ToAString("レベル9.1でデバイスを作成"));

	if (FAILED(device->QueryInterface(__uuidof(IDXGIDevice1), (void**) &dxgiDevice)))
	{
		writeLog(ToAString("デバイス1の作成に失敗"));
		goto End;
	}

	DXGI_SWAP_CHAIN_DESC hDXGISwapChainDesc;
	hDXGISwapChainDesc.BufferDesc.Width = width;
	hDXGISwapChainDesc.BufferDesc.Height = height;
	hDXGISwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	hDXGISwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	//hDXGISwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	hDXGISwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	hDXGISwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	hDXGISwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	hDXGISwapChainDesc.SampleDesc.Count = 1;
	hDXGISwapChainDesc.SampleDesc.Quality = 0;
	hDXGISwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	hDXGISwapChainDesc.BufferCount = 1;
	hDXGISwapChainDesc.OutputWindow = handle;
	hDXGISwapChainDesc.Windowed = isFullScreen ? FALSE : TRUE;
	hDXGISwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	hDXGISwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	if (FAILED(dxgiFactory->CreateSwapChain(device, &hDXGISwapChainDesc, &swapChain)))
	{
		writeLog(ToAString("スワップチェーンの作成に失敗"));
		goto End;
	}

	if (FAILED(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**) &defaultBack)))
	{
		writeLog(ToAString("バックバッファの取得に失敗"));
		goto End;
	}

	if (FAILED(device->CreateRenderTargetView(defaultBack, NULL, &defaultBackRenderTargetView)))
	{
		writeLog(ToAString("バックバッファのレンダーターゲットの取得に失敗"));
		goto End;
	}

	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;


	if (FAILED(device->CreateTexture2D(&descDepth, NULL, &depthBuffer)))
	{
		writeLog(ToAString("深度バッファの作成に失敗"));
		goto End;
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC viewDesc;
	viewDesc.Format = descDepth.Format;
	viewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	viewDesc.Flags = 0;
	if (FAILED(device->CreateDepthStencilView(depthBuffer, &viewDesc, &depthStencilView)))
	{
		writeLog(ToAString("深度バッファのビューの作成に失敗"));
		goto End;
	}

	writeLog(ToAString("DirectX11初期化成功"));
	writeLog(ToAString(""));

	for (size_t i = 0; i < adapters.size(); i++)
	{
		WriteAdapterInformation(log, adapters[i], i);
	}

	// 破棄処理
	for (auto& a : adapters) a->Release();

	return new Graphics_Imp_DX11(
		window,
		Vector2DI(width, height),
		log,
		isReloadingEnabled,
		isFullScreen,
		device,
		context,
		dxgiDevice,
		adapter,
		dxgiFactory,
		swapChain,
		defaultBack,
		defaultBackRenderTargetView,
		depthBuffer,
		depthStencilView);
End:
	SafeRelease(depthBuffer);
	SafeRelease(depthStencilView);
	SafeRelease(swapChain);
	SafeRelease(dxgiFactory);
	SafeRelease(adapter);
	SafeRelease(dxgiDevice);
	SafeRelease(context);
	SafeRelease(device);
	for (auto& a : adapters) a->Release();

	writeLog(ToAString("DirectX11初期化失敗"));
	writeLog(ToAString(""));

	return nullptr;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Graphics_Imp_DX11* Graphics_Imp_DX11::Create(Window* window, Log* log, bool isReloadingEnabled, bool isFullScreen)
{
	auto size = window->GetSize();
	auto handle = glfwGetWin32Window(((Window_Imp*) window)->GetWindow());
	return Create(handle, size.X, size.Y, log, isReloadingEnabled, isFullScreen);
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Graphics_Imp_DX11* Graphics_Imp_DX11::Create(HWND handle, int32_t width, int32_t height, Log* log, bool isReloadingEnabled, bool isFullScreen)
{
	return Create(nullptr, handle, width, height, log, isReloadingEnabled, isFullScreen);
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Texture2D_Imp* Graphics_Imp_DX11::CreateTexture2D_Imp_Internal(Graphics* graphics, uint8_t* data, int32_t size)
{
	auto ret = Texture2D_Imp_DX11::Create(this, data, size, true);
	return ret;
}

Texture2D_Imp* Graphics_Imp_DX11::CreateTexture2DAsRawData_Imp_Internal(Graphics* graphics, uint8_t* data, int32_t size)
{
	auto ret = Texture2D_Imp_DX11::Create(this, data, size, false);
	return ret;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Texture2D_Imp* Graphics_Imp_DX11::CreateEmptyTexture2D_Imp_Internal(Graphics* graphics, int32_t width, int32_t height, TextureFormat format)
{
	auto ret = Texture2D_Imp_DX11::Create(this, width, height, format);
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

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
DepthBuffer_Imp* Graphics_Imp_DX11::CreateDepthBuffer_Imp(int32_t width, int32_t height)
{
	return DepthBuffer_Imp_DX11::Create(this, width, height);
}

void Graphics_Imp_DX11::CommitRenderState(bool forced)
{
	bool changeDepth = forced;
	bool changeRasterizer = forced;
	bool changeBlend = forced;

	auto& current = currentState.renderState;
	auto& next = nextState.renderState;

	auto& currentFilter = currentState.textureFilterTypes;
	auto& nextFilter = nextState.textureFilterTypes;

	auto& currentWrap = currentState.textureWrapTypes;
	auto& nextWrap = nextState.textureWrapTypes;

	auto& currentFilter_vs = currentState.textureFilterTypes_vs;
	auto& nextFilter_vs = nextState.textureFilterTypes_vs;

	auto& currentWrap_vs = currentState.textureWrapTypes_vs;
	auto& nextWrap_vs = nextState.textureWrapTypes_vs;

	if (current.DepthTest != next.DepthTest || forced)
	{
		changeDepth = true;
	}

	if (current.DepthWrite != next.DepthWrite || forced)
	{
		changeDepth = true;
	}

	if (changeDepth)
	{
		GetContext()->OMSetDepthStencilState(m_dStates[next.DepthTest][next.DepthWrite], 0);
	}

	if (current.Culling != next.Culling || forced)
	{
		changeRasterizer = true;
	}

	if (changeRasterizer)
	{
		GetContext()->RSSetState(m_rStates[(int32_t)next.Culling]);
	}

	if (current.AlphaBlendState != next.AlphaBlendState || forced)
	{
		changeBlend = true;
	}

	if (changeBlend)
	{
		float blendFactor [] = { 0, 0, 0, 0 };
		GetContext()->OMSetBlendState(m_bStates[(int32_t) next.AlphaBlendState], blendFactor, 0xFFFFFFFF);
	}

	for (int32_t i = 0; i < MaxTextureCount; i++)
	{
		bool changeSampler = forced;

		if (currentFilter_vs[i] != nextFilter_vs[i] || forced)
		{
			changeSampler = true;
		}

		if (currentWrap_vs[i] != nextWrap_vs[i] || forced)
		{
			changeSampler = true;
		}

		if (changeSampler)
		{
			ID3D11SamplerState* samplerTbl [] = { m_sStates[(int32_t) nextFilter_vs[i]][(int32_t) nextWrap_vs[i]] };
			GetContext()->VSSetSamplers(i, 1, samplerTbl);
		}
	}

	for (int32_t i = 0; i < MaxTextureCount; i++)
	{
		bool changeSampler = forced;

		if (currentFilter[i] != nextFilter[i] || forced)
		{
			changeSampler = true;
		}

		if (currentWrap[i] != nextWrap[i] || forced)
		{
			changeSampler = true;
		}

		if (changeSampler)
		{
			ID3D11SamplerState* samplerTbl [] = { m_sStates[(int32_t) nextFilter[i]][(int32_t) nextWrap[i]] };
			GetContext()->PSSetSamplers(i, 1, samplerTbl);
		}
	}

	currentState = nextState;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Graphics_Imp_DX11::SetRenderTarget(RenderTexture2D_Imp* texture, DepthBuffer_Imp* depthBuffer)
{
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
		rt = ((RenderTexture2D_Imp_DX11*) texture)->GetRenderTargetView();
	}

	if (depthBuffer != nullptr)
	{
		ds = ((DepthBuffer_Imp_DX11*) depthBuffer)->GetDepthStencilView();
	}

	if (rt != nullptr)
	{
		m_context->OMSetRenderTargets(1, &rt, ds);
		SetViewport(0, 0, texture->GetSize().X, texture->GetSize().Y);

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
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Graphics_Imp_DX11::SetRenderTarget(RenderTexture2D_Imp* texture1, RenderTexture2D_Imp* texture2, RenderTexture2D_Imp* texture3, RenderTexture2D_Imp* texture4, DepthBuffer_Imp* depthBuffer)
{
	// 強制リセット(テクスチャと描画先同時設定不可のため)
	for (int32_t i = 0; i < Graphics_Imp::MaxTextureCount; i++)
	{
		ID3D11ShaderResourceView* rv = { nullptr };
		GetContext()->VSSetShaderResources(i, 1, &rv);
		GetContext()->PSSetShaderResources(i, 1, &rv);
	}

	if (texture1 == nullptr)
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

	ID3D11RenderTargetView* rt[MaxRenderTarget] = { nullptr, nullptr, nullptr, nullptr };
	ID3D11DepthStencilView* ds = nullptr;

	if (texture1 != nullptr)
	{
		rt[0] = ((RenderTexture2D_Imp_DX11*) texture1)->GetRenderTargetView();
	}

	if (texture2 != nullptr)
	{
		rt[1] = ((RenderTexture2D_Imp_DX11*) texture2)->GetRenderTargetView();
	}

	if (texture3 != nullptr)
	{
		rt[2] = ((RenderTexture2D_Imp_DX11*) texture3)->GetRenderTargetView();
	}

	if (texture4 != nullptr)
	{
		rt[3] = ((RenderTexture2D_Imp_DX11*) texture4)->GetRenderTargetView();
	}

	if (depthBuffer != nullptr)
	{
		ds = ((DepthBuffer_Imp_DX11*) depthBuffer)->GetDepthStencilView();
	}

	if (rt != nullptr)
	{
		m_context->OMSetRenderTargets(4, rt, ds);
		SetViewport(0, 0, texture1->GetSize().X, texture1->GetSize().Y);

		for (auto i = 0; i < MaxRenderTarget; i++)
		{
			SafeAddRef(rt[i]);
			SafeRelease(m_currentBackRenderTargetViews[i]);
			m_currentBackRenderTargetViews[i] = rt[i];
		}

		SafeRelease(m_currentDepthStencilView);
		m_currentDepthStencilView = ds;
		SafeAddRef(m_currentDepthStencilView);
	}
}

void Graphics_Imp_DX11::SetRenderTarget(CubemapTexture_Imp* texture, int32_t direction, int32_t mipmap, DepthBuffer_Imp* depthBuffer)
{
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
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Graphics_Imp_DX11::SetViewport(int32_t x, int32_t y, int32_t width, int32_t height)
{
	D3D11_VIEWPORT vp;
	vp.TopLeftX = x;
	vp.TopLeftY = y;
	vp.Width = (float) width;
	vp.Height = (float) height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	m_context->RSSetViewports(1, &vp);
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

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Graphics_Imp_DX11::Clear(bool isColorTarget, bool isDepthTarget, const Color& color)
{
	if (isColorTarget)
	{
		float ClearColor [] = { color.R / 255.0f, color.G / 255.0f, color.B / 255.0f, color.A / 255.0f };

		for (auto i = 0; i < MaxRenderTarget; i++)
		{
			if (m_currentBackRenderTargetViews[i] == nullptr) continue;
			m_context->ClearRenderTargetView(m_currentBackRenderTargetViews[i], ClearColor);
		}
	}

	if (isDepthTarget && m_currentDepthStencilView != nullptr)
	{
		m_context->ClearDepthStencilView(m_currentDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Graphics_Imp_DX11::Present()
{
	// 同期しない
	m_swapChain->Present(0, 0);
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Graphics_Imp_DX11::SaveScreenshot(const achar* path)
{
	HRESULT hr;

	ID3D11Resource* resource = nullptr;

	m_defaultBackRenderTargetView->GetResource(&resource);


	SaveTexture(path, resource, m_size);
	
END:;
	SafeRelease(resource);
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
bool Graphics_Imp_DX11::SaveTexture(const achar* path, ID3D11Resource* texture, Vector2DI size)
{
	ID3D11Texture2D* texture_ = nullptr;

	HRESULT hr;

	D3D11_TEXTURE2D_DESC desc;
	desc.ArraySize = 1;
	desc.BindFlags = 0;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.Width = size.X;
	desc.Height = size.Y;
	desc.MipLevels = 1;
	desc.MiscFlags = 0;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_STAGING;

	hr = GetDevice()->CreateTexture2D(&desc, 0, &texture_);
	if (FAILED(hr))
	{
		goto END;
	}

	GetContext()->CopyResource(texture_, texture);

	D3D11_MAPPED_SUBRESOURCE mr;
	UINT sr = D3D11CalcSubresource(0, 0, 0);
	hr = GetContext()->Map(texture_, sr, D3D11_MAP_READ_WRITE, 0, &mr);
	if (FAILED(hr))
	{
		return false;
	}

	auto data = new uint8_t[size.X * size.Y * 4];

	for (int32_t h = 0; h < size.Y; h++)
	{
		auto dst = &(data[h*size.X * 4]);
		auto src = &(((uint8_t*) mr.pData)[h*mr.RowPitch]);
		memcpy(dst, src, size.X * 4);
	}

	ImageHelper::SavePNGImage(path, size.X, size.Y, data, false);

	SafeDeleteArray(data);

	GetContext()->Unmap(texture_, sr);

	SafeRelease(texture_);
	return true;

END:;
	SafeRelease(texture_);
	return false;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

}
