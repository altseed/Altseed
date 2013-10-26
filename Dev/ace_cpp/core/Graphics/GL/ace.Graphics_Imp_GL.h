#pragma once

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "../Common/ace.Graphics_Imp.h"
#include "ace.GL.Base.h"

#if !_WIN32
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <GL/glx.h>
#endif

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	class Graphics_Imp_GL
		: public Graphics_Imp
	{
	private:
		GLFWwindow*		m_window;

		GLuint			m_frameBuffer;

#if !_WIN32
		bool			m_x11Mode = false;
		GLXContext		m_glx;
		Display*		m_x11Display;
		::Window		m_x11Window;
#endif

		Graphics_Imp_GL(Vector2DI size, ::ace::Window* window, Log* log);

		Graphics_Imp_GL(Vector2DI size, void* display, void* window, void* context, Log* log);

		virtual ~Graphics_Imp_GL();

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

		void DrawPolygonInternal(int32_t count, VertexBuffer_Imp* vertexBuffer, IndexBuffer_Imp* indexBuffer, NativeShader_Imp* shaderPtr);
		void BeginInternal();

		void SetViewport(int32_t x, int32_t y, int32_t width, int32_t height);

	public:
		
		static Graphics_Imp_GL* Create(::ace::Window* window, Log* log);

#if !_WIN32
		static Graphics_Imp_GL* Create_X11(void* display, void* window, int32_t width, int32_t height, Log* log);
#endif

		Texture2D_Imp* CreateTexture2D_Imp_Internal(Graphics* graphics, uint8_t* data, int32_t size);
		RenderTexture_Imp* CreateRenderTexture_Imp(int32_t width, int32_t height, eTextureFormat format);
		DepthBuffer_Imp* CreateDepthBuffer_Imp(int32_t width, int32_t height);

		void SetRenderTarget(RenderTexture_Imp* texture, DepthBuffer_Imp* depthBuffer);
	public:
		void Clear(bool isColorTarget, bool isDepthTarget, const Color& color);

		void Present();

		void SaveScreenshot(const achar* path);

		eGraphicsType GetGraphicsType() const { return GRAPHICS_TYPE_GL; }
	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
}
