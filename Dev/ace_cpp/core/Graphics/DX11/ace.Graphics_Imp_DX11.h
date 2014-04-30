#pragma once

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "../Common/ace.Graphics_Imp.h"
#include "ace.DX11.Base.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
#if !SWIG
	class GraphicsHelper_DX11
	{
	public:
		static void LoadTexture(Graphics_Imp_DX11* graphics, void* imgdata, int32_t width, int32_t height, ID3D11Texture2D*& texture, ID3D11ShaderResourceView*& textureSRV);
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

		ID3D11Device*			m_device;
		ID3D11DeviceContext*	m_context;
		IDXGIDevice1*			m_dxgiDevice;
		IDXGIAdapter1*			m_adapter;
		IDXGIFactory1*			m_dxgiFactory;
		IDXGISwapChain*			m_swapChain;

		ID3D11Texture2D*		m_defaultBack;
		ID3D11RenderTargetView*	m_defaultBackRenderTargetView;

		ID3D11Texture2D*		m_defaultDepthBuffer;
		ID3D11DepthStencilView*	m_defaultDepthStencilView;

		ID3D11RenderTargetView*	m_currentBackRenderTargetView;
		ID3D11DepthStencilView*	m_currentDepthStencilView;

		Graphics_Imp_DX11(
			Window* window,
			Vector2DI size,
			Log* log,
			bool isMultithreadingMode,
			ID3D11Device* device,
			ID3D11DeviceContext* context,
			IDXGIDevice1* dxgiDevice,
			IDXGIAdapter1* adapter,
			IDXGIFactory1* dxgiFactory,
			IDXGISwapChain* swapChain,
			ID3D11Texture2D* defaultBack,
			ID3D11RenderTargetView*	defaultBackRenderTargetView,
			ID3D11Texture2D* defaultDepthBuffer,
			ID3D11DepthStencilView* defaultDepthStencilView);
		virtual ~Graphics_Imp_DX11();

		static void WriteAdapterInformation(Log* log, IDXGIAdapter1* adapter, int32_t index);

	protected:
		VertexBuffer_Imp* CreateVertexBuffer_Imp_(int32_t size, int32_t count, bool isDynamic);
		IndexBuffer_Imp* CreateIndexBuffer_Imp_(int maxCount, bool isDynamic, bool is32bit);
		NativeShader_Imp* CreateShader_Imp_(
			const char* vertexShaderText,
			const char* vertexShaderFileName,
			const char* pixelShaderText,
			const char* pixelShaderFileName,
			std::vector <VertexLayout>& layout,
			std::vector <Macro>& macro);

		void UpdateDrawStates(VertexBuffer_Imp* vertexBuffer, IndexBuffer_Imp* indexBuffer, NativeShader_Imp* shaderPtr, int32_t& vertexBufferOffset);
		void DrawPolygonInternal(int32_t count, VertexBuffer_Imp* vertexBuffer, IndexBuffer_Imp* indexBuffer, NativeShader_Imp* shaderPtr);
		void DrawPolygonInstancedInternal(int32_t count, VertexBuffer_Imp* vertexBuffer, IndexBuffer_Imp* indexBuffer, NativeShader_Imp* shaderPtr, int32_t instanceCount);

		void BeginInternal();

		static Graphics_Imp_DX11* Create(Window* window, HWND handle, int32_t width, int32_t height, Log* log, bool isMultithreadingMode);

	public:
		
		static Graphics_Imp_DX11* Create(Window* window, Log* log, bool isMultithreadingMode);

		static Graphics_Imp_DX11* Create(HWND handle, int32_t width, int32_t height, Log* log, bool isMultithreadingMode);

		
		Texture2D_Imp* CreateTexture2D_Imp_Internal(Graphics* graphics, uint8_t* data, int32_t size);

		Texture2D_Imp* CreateEmptyTexture2D_Imp_Internal(Graphics* graphics, int32_t width, int32_t height, eTextureFormat format) override;

		RenderTexture2D_Imp* CreateRenderTexture2D_Imp(int32_t width, int32_t height, eTextureFormat format);

		CubemapTexture* CreateCubemapTextureFrom6ImageFiles_(const achar* front, const achar* left, const achar* back, const achar* right, const achar* top, const achar* bottom) override;

		DepthBuffer_Imp* CreateDepthBuffer_Imp(int32_t width, int32_t height);

		void SetRenderTarget(RenderTexture2D_Imp* texture, DepthBuffer_Imp* depthBuffer);

		void SetViewport(int32_t x, int32_t y, int32_t width, int32_t height);

		void MakeContextCurrent();

		void FlushCommand();

	public:
		void Clear(bool isColorTarget, bool isDepthTarget, const Color& color);

		void Present();

		void SaveScreenshot(const achar* path);

		bool SaveTexture(const achar* path, ID3D11Resource* texture, Vector2DI size);

	public:

		ID3D11Device* GetDevice() { return m_device; }
		ID3D11DeviceContext* GetContext() { return m_context;}

		eGraphicsType GetGraphicsType() const { return GRAPHICS_TYPE_DX11; }
	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
}