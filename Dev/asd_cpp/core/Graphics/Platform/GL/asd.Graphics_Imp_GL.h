#pragma once

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "../../asd.Graphics_Imp.h"
#include "asd.GL.Base.h"

#include <mutex>

#if _WIN32
#elif __APPLE__
#else
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <GL/glx.h>
#endif

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace asd {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
#if !SWIG
	class GraphicsHelper_GL
	{
	public:
		static void LoadTexture(Graphics_Imp_GL* graphics, void* imgdata, int32_t width, int32_t height, GLuint& texture);
	};

	class DistortingCallbackGL
		: public EffectDistortingCallback
	{
		GLuint framebufferForCopy = 0;
		GLuint backGroundTexture = 0;
		uint32_t backGroundTextureWidth = 0;
		uint32_t backGroundTextureHeight = 0;
		GLuint backGroundTextureInternalFormat = 0;

		EffekseerRendererGL::Renderer*	renderer = nullptr;

	public:
		DistortingCallbackGL(EffekseerRendererGL::Renderer* renderer);
		virtual ~DistortingCallbackGL();
		void ReleaseTexture();
		void PrepareTexture(uint32_t width, uint32_t height, GLint internalFormat);
		virtual void OnDistorting() override;
	};
#endif

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	class Graphics_Imp_GL
		: public Graphics_Imp
	{
	private:
		Window*			m_window;
		
		GLuint			m_frameBuffer;

#ifdef __APPLE__
		GLuint			m_vao;
#endif

		bool			m_endStarting;
#if _WIN32
#elif __APPLE__
#else
		bool			m_x11Mode = false;
		GLXContext		m_glx;
		Display*		m_x11Display;
		::Window		m_x11Window;
#endif


#pragma region RenderState
		GLuint			m_samplers[MaxTextureCount];
#pragma endregion

		Graphics_Imp_GL(Vector2DI size, ::asd::Window* window, Log* log, File* file, GraphicsOption option);

		Graphics_Imp_GL(Vector2DI size, void* display, void* window, void* context, Log* log, File* file, GraphicsOption option);

		virtual ~Graphics_Imp_GL();

		static void WriteInitializedLog(Log* log);
		static void WriteDeviceInformation(Log* log);

	protected:
		void StartRenderingThread() override;
		void EndRenderingThread() override;

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

		void UpdateStatus(VertexBuffer_Imp* vertexBuffer, IndexBuffer_Imp* indexBuffer, NativeShader_Imp* shaderPtr);
		void DrawPolygonInternal(int32_t count, VertexBuffer_Imp* vertexBuffer, IndexBuffer_Imp* indexBuffer, NativeShader_Imp* shaderPtr);
		void DrawPolygonInternal(int32_t offset, int32_t count, VertexBuffer_Imp* vertexBuffer, IndexBuffer_Imp* indexBuffer, NativeShader_Imp* shaderPtr) override;
		void DrawPolygonInstancedInternal(int32_t count, VertexBuffer_Imp* vertexBuffer, IndexBuffer_Imp* indexBuffer, NativeShader_Imp* shaderPtr, int32_t instanceCount);

		void BeginInternal();
		void EndInternal();

		void SetViewport(int32_t x, int32_t y, int32_t width, int32_t height);

	public:
		
		static Graphics_Imp_GL* Create(::asd::Window* window, Log* log, File* file, GraphicsOption option);

#if _WIN32
#elif __APPLE__
#else
		static Graphics_Imp_GL* Create_X11(void* display, void* window, int32_t width, int32_t height, Log* log, File* file, GraphicsOption option);
#endif

		Texture2D_Imp* CreateTexture2D_Imp_Internal(Graphics* graphics, uint8_t* data, int32_t size) override;
		Texture2D_Imp* CreateTexture2DAsRawData_Imp_Internal(Graphics* graphics, uint8_t* data, int32_t size) override;

		Texture2D_Imp* CreateEmptyTexture2D_Imp_Internal(Graphics* graphics, int32_t width, int32_t height, TextureFormat format, void* data) override;

		Texture2D_Imp* CreateEditableTexture2D_Imp_Internal(Graphics* graphics, uint8_t* data, int32_t size) override;

		RenderTexture2D_Imp* CreateRenderTexture2D_Imp(int32_t width, int32_t height, TextureFormat format);

		CubemapTexture* CreateCubemapTextureFrom6ImageFiles_(const achar* front, const achar* left, const achar* back, const achar* right, const achar* top, const achar* bottom) override;

		CubemapTexture* CreateCubemapTextureFromMipmapImageFiles_(const achar* path, int32_t mipmapCount) override;

		CubemapTexture* CreateCubemapTextureFromSingleImageFile_(const achar* path) override;

		DepthBuffer_Imp* CreateDepthBuffer_Imp(int32_t width, int32_t height);

		void CommitRenderState(bool forced) override;

		void SetRenderTarget(RenderTexture2D_Imp* texture, DepthBuffer_Imp* depthBuffer);

		void SetRenderTarget(RenderTexture2D_Imp* texture1, RenderTexture2D_Imp* texture2, RenderTexture2D_Imp* texture3, RenderTexture2D_Imp* texture4, DepthBuffer_Imp* depthBuffer);

		void SetRenderTarget(CubemapTexture_Imp* texture, int32_t direction, int32_t mipmap, DepthBuffer_Imp* depthBuffer) override;

	public:
		void Clear(bool isColorTarget, bool isDepthTarget, const Color& color);

		void Present();

		void SaveScreenshot(const achar* path);

		void SaveScreenshot(std::vector<Color>& bufs, Vector2DI& size);

		bool SaveTexture(const achar* path, GLuint texture, Vector2DI size);

		bool SaveTexture(std::vector<Color>& bufs, GLuint texture, Vector2DI size);

		GraphicsDeviceType GetGraphicsDeviceType() const { return GraphicsDeviceType::OpenGL; }

		/**
			@brief	現状のスレットに対応したコンテキストを設定する。
		*/
		void MakeContextCurrent() override;

		/**
		@brief	コンテキストを解除する。
		*/
		void MakeContextNone();

		void FlushCommand() override;

		void SetIsFullscreenMode(bool isFullscreenMode) override;

		void SetWindowSize(Vector2DI size) override;

	private:

		/**
		@brief	描画スレッド用にコンテキストを生成する。(描画スレッド生成前)
		@param	window	ウインドウ
		*/
		void CreateContextBeforeThreading(Window* window);

		/**
			@brief	描画スレッド用にコンテキストを生成する。(描画スレッド上)
			@param	window	ウインドウ
		*/
		void CreateContextOnThread(Window* window);

		/**
		@brief	描画スレッド用にコンテキストを生成する。(描画スレッド生成後)
		@param	window	ウインドウ
		*/
		void CreateContextAfterThreading(Window* window);

		/**
			@brief	フレームバッファをバインドする。
			@note
			フレームバッファはコンテキスト間で共有できないため、スレッドに応じて切り替えている。
		*/
		void BindFramebuffer();

		/**
			@brief	フレームバッファをアンバインドする。
		*/
		void UnbindFramebuffer();
	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
}
