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
	class Graphics_Imp_DX11
		: public Graphics_Imp
	{
	private:
		Window*					m_window;

		ID3D11Device*			m_device;
		ID3D11DeviceContext*	m_context;
		IDXGIDevice1*			m_dxgiDevice;
		IDXGIAdapter*			m_adapter;
		IDXGIFactory*			m_dxgiFactory;
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
			ID3D11Device* device,
			ID3D11DeviceContext* context,
			IDXGIDevice1* dxgiDevice,
			IDXGIAdapter* adapter,
			IDXGIFactory* dxgiFactory,
			IDXGISwapChain* swapChain,
			ID3D11Texture2D* defaultBack,
			ID3D11RenderTargetView*	defaultBackRenderTargetView,
			ID3D11Texture2D* defaultDepthBuffer,
			ID3D11DepthStencilView* defaultDepthStencilView);
		virtual ~Graphics_Imp_DX11();

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
		void BeginInternal();

		static Graphics_Imp_DX11* Create(Window* window, HWND handle, int32_t width, int32_t height, Log* log);

	public:
		
		static Graphics_Imp_DX11* Create(Window* window, Log* log);

		static Graphics_Imp_DX11* Create(HWND handle, int32_t width, int32_t height, Log* log);

		
		Texture2D_Imp* CreateTexture2D_Imp_Internal(Graphics* graphics, uint8_t* data, int32_t size);

		RenderTexture_Imp* CreateRenderTexture_Imp(int32_t width, int32_t height, eTextureFormat format);

		DepthBuffer_Imp* CreateDepthBuffer_Imp(int32_t width, int32_t height);

		void SetRenderTarget(RenderTexture_Imp* texture, DepthBuffer_Imp* depthBuffer);

		void SetViewport(int32_t x, int32_t y, int32_t width, int32_t height);

		void MakeContextCurrent();

		void FlushCommand();

	public:
		void Clear(bool isColorTarget, bool isDepthTarget, const Color& color);

		void Present();

		void SaveScreenshot(const achar* path);

		ID3D11Device* GetDevice() { return m_device; }
		ID3D11DeviceContext* GetContext() { return m_context;}

		eGraphicsType GetGraphicsType() const { return GRAPHICS_TYPE_DX11; }
	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
}