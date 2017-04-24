#pragma once

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "../../asd.Graphics_Imp.h"
#include "asd.DX11.Base.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace asd {

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
#if !SWIG
	class GraphicsHelper_DX11
	{
	public:
		static void LoadTexture(Graphics_Imp_DX11* graphics, void* imgdata, int32_t width, int32_t height, ID3D11Texture2D*& texture, ID3D11ShaderResourceView*& textureSRV);

		static astring GetErrorMessage(Graphics_Imp_DX11* graphics, HRESULT hr);

		static std::string GetFormatName(Graphics_Imp_DX11* graphics, DXGI_FORMAT format);

		static TextureFormat GetTextureFormat(DXGI_FORMAT format);
	};

	class DistortingCallbackDX11
		: public EffectDistortingCallback
	{
		::EffekseerRendererDX11::Renderer* renderer = nullptr;
		ID3D11Texture2D* backGroundTexture = nullptr;
		ID3D11ShaderResourceView* backGroundTextureSRV = nullptr;
		D3D11_TEXTURE2D_DESC backGroundTextureDesc = {};

		ID3D11Device*			g_D3d11Device = NULL;
		ID3D11DeviceContext*	g_D3d11Context = NULL;


	public:
		DistortingCallbackDX11(::EffekseerRendererDX11::Renderer* renderer, ID3D11Device* d3d11Device, ID3D11DeviceContext* d3d11Context);
		virtual ~DistortingCallbackDX11();
		void ReleaseTexture();

		// コピー先のテクスチャを準備
		void PrepareTexture(uint32_t width, uint32_t height, DXGI_FORMAT format);

		virtual void OnDistorting() override;
	};
#endif

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	class Graphics_Imp_DX11
		: public Graphics_Imp
	{
	private:
		Window*					m_window;
		ar::Context*			rhiContext = nullptr;
		ar::DrawParameter		drawParam;

		bool					isSceneRunning = false;
		bool					isRenderTargetDirty = false;

		std::array<RenderTexture2D_Imp*, 4>	renderTargets;
		DepthBuffer_Imp*	depthTarget = nullptr;

		std::array<RenderTexture2D_Imp*, 4>	currentRenderTargets;
		DepthBuffer_Imp*	currentDepthTarget = nullptr;

		bool					isInitializedAsDX9 = false;

		Graphics_Imp_DX11(
			ar::Manager* manager,
			Window* window,
			Vector2DI size,
			Log* log,
			File* file,
			GraphicsOption option);
		virtual ~Graphics_Imp_DX11();

		static void WriteAdapterInformation(Log* log, IDXGIAdapter1* adapter, int32_t index);

		void ApplyRenderTargets();

	protected:
		VertexBuffer_Imp* CreateVertexBuffer_Imp_(int32_t size, int32_t count, bool isDynamic);
		IndexBuffer_Imp* CreateIndexBuffer_Imp_(int maxCount, bool isDynamic, bool is32bit);
		NativeShader_Imp* CreateShader_Imp_(
			const char* vertexShaderText,
			const char* vertexShaderFileName,
			const char* pixelShaderText,
			const char* pixelShaderFileName,
			std::vector <VertexLayout>& layout,
			bool is32Bit,
			std::vector <Macro>& macro);

		void UpdateDrawStates(VertexBuffer_Imp* vertexBuffer, IndexBuffer_Imp* indexBuffer, NativeShader_Imp* shaderPtr, int32_t& vertexBufferOffset);
		void DrawPolygonInternal(int32_t count, VertexBuffer_Imp* vertexBuffer, IndexBuffer_Imp* indexBuffer, NativeShader_Imp* shaderPtr);
		void DrawPolygonInternal(int32_t offset, int32_t count, VertexBuffer_Imp* vertexBuffer, IndexBuffer_Imp* indexBuffer, NativeShader_Imp* shaderPtr) override;

		void DrawPolygonInstancedInternal(int32_t count, VertexBuffer_Imp* vertexBuffer, IndexBuffer_Imp* indexBuffer, NativeShader_Imp* shaderPtr, int32_t instanceCount);

		void BeginInternal();

		void EndInternal();

		static Graphics_Imp_DX11* Create(Window* window, HWND handle, int32_t width, int32_t height, Log* log, File *file, GraphicsOption option);

	public:
		
		static Graphics_Imp_DX11* Create(Window* window, Log* log, File* file, GraphicsOption option);

		static Graphics_Imp_DX11* Create(HWND handle, int32_t width, int32_t height, Log* log, File* file, GraphicsOption option);

		Texture2D_Imp* CreateTexture2D_Imp_Internal(Graphics* graphics, uint8_t* data, int32_t size);

		Texture2D_Imp* CreateTexture2DAsRawData_Imp_Internal(Graphics* graphics, uint8_t* data, int32_t size);

		Texture2D_Imp* CreateEmptyTexture2D_Imp_Internal(Graphics* graphics, int32_t width, int32_t height, TextureFormat format, void* data) override;

		Texture2D_Imp* CreateEditableTexture2D_Imp_Internal(Graphics* graphics, uint8_t* data, int32_t size);

		RenderTexture2D_Imp* CreateRenderTexture2D_Imp(int32_t width, int32_t height, TextureFormat format);

		CubemapTexture* CreateCubemapTextureFrom6ImageFiles_(const achar* front, const achar* left, const achar* back, const achar* right, const achar* top, const achar* bottom) override;

		CubemapTexture* CreateCubemapTextureFromMipmapImageFiles_(const achar* path, int32_t mipmapCount) override;

		CubemapTexture* CreateCubemapTextureFromSingleImageFile_(const achar* path) override;

		DepthBuffer_Imp* CreateDepthBuffer_Imp(int32_t width, int32_t height);

		void CommitRenderState(bool forced) override;

		void SetRenderTarget(RenderTexture2D_Imp* texture, DepthBuffer_Imp* depthBuffer);

		void SetRenderTarget(RenderTexture2D_Imp* texture1, RenderTexture2D_Imp* texture2, RenderTexture2D_Imp* texture3, RenderTexture2D_Imp* texture4, DepthBuffer_Imp* depthBuffer);

		void SetRenderTarget(CubemapTexture_Imp* texture, int32_t direction, int32_t mipmap, DepthBuffer_Imp* depthBuffer) override;

		void MakeContextCurrent();

		void FlushCommand();

		void SetIsFullscreenMode(bool isFullscreenMode) override;

		void SetWindowSize(Vector2DI size) override;

	public:
		void Clear(bool isColorTarget, bool isDepthTarget, const Color& color);

		void Present();

		void SaveScreenshot(const achar* path);

		void SaveScreenshot(std::vector<Color>& bufs, Vector2DI& size);

	public:

		bool GetIsInitializedAsDX9() { return isInitializedAsDX9; }
		ID3D11Device* GetDevice() { return (ID3D11Device*)GetRHI()->GetInternalObjects()[0]; }
		ID3D11DeviceContext* GetContext() { return (ID3D11DeviceContext*)GetRHI()->GetInternalObjects()[1]; }

		GraphicsDeviceType GetGraphicsDeviceType() const { return (GraphicsDeviceType)GetRHI()->GetDeviceType(); }
	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
}