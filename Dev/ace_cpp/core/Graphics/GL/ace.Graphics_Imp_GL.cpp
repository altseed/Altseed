
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "ace.Graphics_Imp_GL.h"

#include "../Common/ace.RenderingThread.h"

#include "../../Window/ace.Window_Imp.h"
#include "Resource/ace.Texture2D_Imp_GL.h"
#include "Resource/ace.VertexBuffer_Imp_GL.h"
#include "Resource/ace.IndexBuffer_Imp_GL.h"
#include "Resource/ace.NativeShader_Imp_GL.h"
#include "Resource/ace.RenderState_Imp_GL.h"
#include "Resource/ace.RenderTexture_Imp_GL.h"
#include "Resource/ace.DepthBuffer_Imp_GL.h"

// Windows以外でGoogleTestとGLFWNativeのヘッダが干渉する(2013/12)
#include <GLFW/glfw3native.h>

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Graphics_Imp_GL::Graphics_Imp_GL(Vector2DI size, ::ace::Window* window, Log* log)
	: Graphics_Imp(size, log)
	, m_window(window)
	, m_endStarting(false)
{
	assert(window != nullptr);
	SafeAddRef(m_window);

#if _WIN32
	m_renderingThreadDC = 0;
	m_renderingThreadRC = 0;
	m_renderingThreadHWND = nullptr;
#else
	m_renderingThreadGlx = nullptr;
	m_renderingThreadX11Display = nullptr;
	m_renderingThreadX11Window = 0;
#endif

	auto window_ = ((Window_Imp*)window)->GetWindow();

	glfwMakeContextCurrent(window_);

	glewInit();

	m_renderState = new RenderState_Imp_GL(this);

	// フレームバッファ生成
	glGenFramebuffers(1, &m_frameBuffer);

	// バグ対策？
	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// スレッド生成
	glfwMakeContextCurrent(nullptr);
	CreateContextBeforeThreading(window_);
	m_renderingThread->Run(this, StartRenderingThreadFunc, EndRenderingThreadFunc);
	while (!m_endStarting)
	{
		Sleep(1);
	}
	CreateContextAfterThreading(window_);
	glfwMakeContextCurrent(window_);
	GLCheckError();
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Graphics_Imp_GL::Graphics_Imp_GL(Vector2DI size, void* display, void* window, void* context, Log* log)
	: Graphics_Imp(size, log)
	, m_window(nullptr)
	, m_endStarting(false)
{
#if !_WIN32
	GLXContext* context_ = (GLXContext*)context;
	m_glx = *context_;

	Display* display_ = (Display*)display;
	::Window window_ = *((::Window*)window);
	
	m_x11Display = display_;
	m_x11Window = window_;

	glXMakeCurrent(m_x11Display, m_x11Window, m_glx);

	m_x11Mode = true;
#endif

#if _WIN32
	m_renderingThreadDC = 0;
	m_renderingThreadRC = 0;
	m_renderingThreadHWND = nullptr;
#else
	m_renderingThreadGlx = nullptr;
	m_renderingThreadX11Display = nullptr;
	m_renderingThreadX11Window = 0;
#endif

	glewInit();

	m_renderState = new RenderState_Imp_GL(this);

	// フレームバッファ生成
	glGenFramebuffers(1, &m_frameBuffer);

	// バグ対策？
	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// スレッド生成
	glfwMakeContextCurrent(nullptr);
	CreateContextBeforeThreading(nullptr);
	m_renderingThread->Run(this, StartRenderingThreadFunc, EndRenderingThreadFunc);
	while (!m_endStarting)
	{
		Sleep(1);
	}
	CreateContextAfterThreading(nullptr);
	glfwMakeContextCurrent(nullptr);
	GLCheckError();
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Graphics_Imp_GL::~Graphics_Imp_GL()
{

	// 必要なし
	//SafeRelease(m_currentShader);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDeleteFramebuffers(1, &m_frameBuffer);

	m_renderingThread->AddEvent(nullptr);
	while (m_renderingThread->IsRunning())
	{
		Sleep(1);
	}
	m_renderingThread.reset();


#if !_WIN32
	if( m_x11Mode )
	{
		glXMakeCurrent(m_x11Display, 0, NULL);
		glXDestroyContext(m_x11Display, m_glx);
	}
#endif

#if _WIN32
	if (m_renderingThreadRC)
	{
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(m_renderingThreadRC);
		m_renderingThreadRC = NULL;
	}
	if (m_renderingThreadDC)
	{
		ReleaseDC(m_renderingThreadHWND, m_renderingThreadDC);
		m_renderingThreadDC = NULL;
		m_renderingThreadHWND = NULL;
	}

#else
	if(m_renderingThreadX11Display != nullptr)
	{
		glXMakeCurrent(m_renderingThreadX11Display, 0, NULL);
		glXDestroyContext(m_renderingThreadX11Display, m_renderingThreadGlx);
		m_renderingThreadX11Display = nullptr;
	}
#endif

	SafeRelease(m_window);
}

void Graphics_Imp_GL::StartRenderingThread()
{
	if (m_window != nullptr)
	{
		auto window_ = ((Window_Imp*) m_window)->GetWindow();
		CreateContextOnThread(window_);
	}
	else
	{
		assert(0);
	}
}

void Graphics_Imp_GL::EndRenderingThread()
{
	if (m_window != nullptr)
	{
#if !_WIN32
		
#endif
	}
	else
	{
		
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
VertexBuffer_Imp* Graphics_Imp_GL::CreateVertexBuffer_Imp_(int32_t size, int32_t count, bool isDynamic)
{
	return VertexBuffer_Imp_GL::Create(this, size, count, isDynamic);
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
IndexBuffer_Imp* Graphics_Imp_GL::CreateIndexBuffer_Imp_(int maxCount, bool isDynamic, bool is32bit)
{
	return IndexBuffer_Imp_GL::Create(this, maxCount, isDynamic, is32bit);
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
NativeShader_Imp* Graphics_Imp_GL::CreateShader_Imp_(
	const char* vertexShaderText,
	const char* vertexShaderFileName,
	const char* pixelShaderText,
	const char* pixelShaderFileName,
	std::vector <VertexLayout>& layout,
	std::vector <Macro>& macro)
{
	return NativeShader_Imp_GL::Create(
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
void Graphics_Imp_GL::DrawPolygonInternal(int32_t count, VertexBuffer_Imp* vertexBuffer, IndexBuffer_Imp* indexBuffer, NativeShader_Imp* shaderPtr)
{
	assert(vertexBuffer != nullptr);
	assert(indexBuffer != nullptr);
	assert(shaderPtr != nullptr);


	auto shader = (NativeShader_Imp_GL*) shaderPtr;
	auto vb = (VertexBuffer_Imp_GL*) vertexBuffer;
	auto ib = (IndexBuffer_Imp_GL*) indexBuffer;

	{
		auto buf = vb->GetBuffer();

		glBindBuffer(GL_ARRAY_BUFFER, buf);

		GLCheckError();
	}

	{
		auto buf = ib->GetBuffer();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buf);
		GLCheckError();
	}

	{
		auto program = shader->GetProgram();

		// シェーダーの設定
		glUseProgram(program);

		// レイアウトの設定
		shader->SetLayout();

		// 定数バッファの設定
		shader->AssignConstantBuffer();

		// テクスチャの設定
		for (int32_t i = 0; i < NativeShader_Imp::TextureCountMax; i++)
		{
			Texture2D* tex = nullptr;
			char* texName = nullptr;
			if (shader->GetTexture(texName, tex, i))
			{
				GLuint buf = 0;
				if (tex->GetType() == TEXTURE_CLASS_TEXTURE2D)
				{
					auto t = (Texture2D_Imp_GL*) tex;
					buf = t->GetBuffer();
				}
				else if (tex->GetType() == TEXTURE_CLASS_RENDERTEXTURE)
				{
					auto t = (RenderTexture_Imp_GL*) tex;
					buf = t->GetBuffer();
				}

				glActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(GL_TEXTURE_2D, buf);

				auto id = glGetUniformLocation(program, texName);
				glUniform1i(id, i);
			}
		}

		GLCheckError();
	}

	// glBindTextureをするたびにテクスチャの値がリセットされるらしいので値を再設定
	GetRenderState()->Update(true);

	if (indexBuffer->Is32Bit())
	{
		glDrawElements(GL_TRIANGLES, count * 3, GL_UNSIGNED_INT, NULL);
	}
	else
	{
		glDrawElements(GL_TRIANGLES, count * 3, GL_UNSIGNED_SHORT, NULL);
	}

	{
		auto shader = (NativeShader_Imp_GL*) shaderPtr;
		shader->Disable();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	GLCheckError();
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Graphics_Imp_GL::BeginInternal()
{
	std::lock_guard<std::recursive_mutex> lock(GetMutex());
	MakeContextCurrent();

	GLCheckError();

	// 描画先のリセット
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDrawBuffer(GL_BACK);

	GLCheckError();
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Graphics_Imp_GL::SetViewport(int32_t x, int32_t y, int32_t width, int32_t height)
{
	std::lock_guard<std::recursive_mutex> lock(GetMutex());
	MakeContextCurrent();

	glViewport(x, y, width, height);
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Graphics_Imp_GL* Graphics_Imp_GL::Create(::ace::Window* window, Log* log)
{
	return new Graphics_Imp_GL(window->GetSize(), window, log);
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#if !_WIN32
Graphics_Imp_GL* Graphics_Imp_GL::Create_X11(void* display, void* window, int32_t width, int32_t height, Log* log )
{
	Display* display_ = (Display*)display;
	::Window window_ = *((::Window*)window);

	GLint attribute[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
	XVisualInfo* vi = glXChooseVisual(display_, DefaultScreen(display_), attribute);

	if( vi == nullptr )
	{	
		return nullptr;
	}

	GLXContext context = glXCreateContext(display_, vi, 0, GL_TRUE);
	GLXContext* context_ = &context;

	XFree(vi);

	return new Graphics_Imp_GL( ace::Vector2DI(width,height), display, window, context_, log );
}
#endif

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Texture2D_Imp* Graphics_Imp_GL::CreateTexture2D_Imp_Internal(Graphics* graphics, uint8_t* data, int32_t size)
{
	auto ret = Texture2D_Imp_GL::Create(this, data, size);
	return ret;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
RenderTexture_Imp* Graphics_Imp_GL::CreateRenderTexture_Imp(int32_t width, int32_t height, eTextureFormat format)
{
	return RenderTexture_Imp_GL::Create(this, width, height);
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
DepthBuffer_Imp* Graphics_Imp_GL::CreateDepthBuffer_Imp(int32_t width, int32_t height)
{
	return DepthBuffer_Imp_GL::Create(this, width, height);
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Graphics_Imp_GL::SetRenderTarget(RenderTexture_Imp* texture, DepthBuffer_Imp* depthBuffer)
{
	std::lock_guard<std::recursive_mutex> lock(GetMutex());
	MakeContextCurrent();

	// 強制リセット
	ResetDrawState();
	for (int32_t i = 0; i < NativeShader_Imp::TextureCountMax; i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	glActiveTexture(GL_TEXTURE0);

	if (texture == nullptr)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDrawBuffer(GL_BACK);
		GetRenderState()->Update(true);
		SetViewport(0, 0, m_size.X, m_size.Y);
		GLCheckError();
		return;
	}

	GLuint cb = 0;
	GLuint db = 0;

	if (texture != nullptr)
	{
		cb = ((RenderTexture_Imp_GL*) texture)->GetBuffer();
	}

	if (depthBuffer != nullptr)
	{
		db = ((DepthBuffer_Imp_GL*) depthBuffer)->GetBuffer();
	}

	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, cb, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, 0, 0);

	if (depthBuffer != nullptr)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, db, 0);
	}
	else
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, 0, 0);
	}

	static const GLenum bufs [] = {
		GL_COLOR_ATTACHMENT0,
	};
	glDrawBuffers(1, bufs);
	GetRenderState()->Update(true);

	if (texture != nullptr)
	{
		SetViewport(0, 0, texture->GetSize().X, texture->GetSize().Y);
	}
	GLCheckError();
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Graphics_Imp_GL::Clear(bool isColorTarget, bool isDepthTarget, const Color& color)
{
	std::lock_guard<std::recursive_mutex> lock(GetMutex());
	MakeContextCurrent();
	
	GLbitfield bit = 0;
	if (isColorTarget)
	{
		bit = bit | GL_COLOR_BUFFER_BIT;
		glClearColor(color.R / 255.0f, color.G / 255.0f, color.B / 255.0f, color.A / 255.0f);
	}
	
	if (isDepthTarget)
	{
		// GL_DEPTH_TEST & WRITE を有効化しないとクリアできない
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);

		bit = bit | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT;
		glClearDepth(1.0f);
	}

	if (bit != 0)
	{
		glClear(bit);
	}

	if (isDepthTarget)
	{
		GetRenderState()->Update(true);
	}

	GLCheckError();
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Graphics_Imp_GL::Present()
{
	std::lock_guard<std::recursive_mutex> lock(GetMutex());
	MakeContextCurrent();

	GLCheckError();

	// Save FPS
	ace::Sleep(16);

	if( m_window !=nullptr )
	{
		auto window_ = ((Window_Imp*) m_window)->GetWindow();
		glfwSwapBuffers(window_);
	}
	else
	{
#if !_WIN32
		glXSwapBuffers(m_x11Display, m_x11Window);
#endif
	}

	GLCheckError();
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Graphics_Imp_GL::SaveScreenshot(const achar* path)
{
	std::lock_guard<std::recursive_mutex> lock(GetMutex());
	MakeContextCurrent();

	GLCheckError();

	glFlush();
	glFinish();

	glViewport( 0, 0, m_size.X, m_size.Y);
	glReadBuffer(GL_BACK);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);


	auto buf = new uint8_t[m_size.X * m_size.Y * 4];

	glReadPixels(
		0,
		0,
		m_size.X,
		m_size.Y,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		(void*)buf);

	SavePNGImage(path, m_size.X, m_size.Y, buf, true);

	SafeDeleteArray(buf);

	GLCheckError();
}

void Graphics_Imp_GL::MakeContextCurrent()
{
	if (GetThreadID() != m_renderingThread->GetThreadID())
	{
		if (m_window != nullptr)
		{
			auto window_ = ((Window_Imp*) m_window)->GetWindow();
			glfwMakeContextCurrent(window_);
		}
		else
		{
			assert(0);
		}
	}
	else
	{
#if _WIN32
		wglMakeCurrent(m_renderingThreadDC, m_renderingThreadRC);
#else
		glXMakeCurrent(m_renderingThreadX11Display, m_renderingThreadX11Window, m_renderingThreadGlx);
#endif
	}
}

void Graphics_Imp_GL::CreateContextBeforeThreading(GLFWwindow* window)
{
	if (window == nullptr) return;

#if _WIN32
	m_renderingThreadHWND = glfwGetWin32Window(window);
	HGLRC mainRC = glfwGetWGLContext(window);

	PIXELFORMATDESCRIPTOR pformat = {
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		0 |
		PFD_DRAW_TO_WINDOW |
		PFD_SUPPORT_OPENGL |
		PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		32,				// color
		0, 0,			// R
		0, 0,			// G
		0, 0,			// B
		0, 0,			// A
		0, 0, 0, 0, 0,      // AC R G B A
		24,				// depth
		8,				// stencil
		0,				// aux
		0,				// layertype
		0,			// reserved
		0,			// layermask
		0,			// visiblemask
		0			// damagemask
	};

	m_renderingThreadDC = GetDC(m_renderingThreadHWND);

	int pfmt = ChoosePixelFormat(m_renderingThreadDC, &pformat);
	SetPixelFormat(m_renderingThreadDC, pfmt, &pformat);
	m_renderingThreadRC = wglCreateContext(m_renderingThreadDC);
	wglShareLists(mainRC, m_renderingThreadRC);
#else

	auto mainContext = glfwGetGLXContext(window);
	auto display = glfwGetX11Display();
	auto wind = glfwGetX11Window(window);

	GLint attribute[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
	XVisualInfo* vi = glXChooseVisual(display, DefaultScreen(display), attribute);

	GLXContext renderingContext = glXCreateContext(display, vi, mainContext, GL_TRUE);
	
	XFree(vi);
	
	m_renderingThreadGlx = renderingContext;
	m_renderingThreadX11Display = display;
	m_renderingThreadX11Window = wind;

#endif

}

void Graphics_Imp_GL::CreateContextOnThread(GLFWwindow* window)
{
	if (window == nullptr) return;

#if _WIN32
#else

#endif

	m_endStarting = true;
}

void Graphics_Imp_GL::CreateContextAfterThreading(GLFWwindow* window)
{
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

}
