
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "ace.Graphics_Imp_GL.h"
#include "../../Window/ace.Window_Imp.h"
#include "Resource/ace.Texture2D_Imp_GL.h"
#include "Resource/ace.VertexBuffer_Imp_GL.h"
#include "Resource/ace.IndexBuffer_Imp_GL.h"
#include "Resource/ace.NativeShader_Imp_GL.h"
#include "Resource/ace.RenderState_Imp_GL.h"
#include "Resource/ace.RenderTexture_Imp_GL.h"
#include "Resource/ace.DepthBuffer_Imp_GL.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Graphics_Imp_GL::Graphics_Imp_GL(Vector2DI size, ::ace::Window* window, Log* log)
	: Graphics_Imp(size, log)
	, m_window(nullptr)
{
	assert(window != nullptr);

	m_window = ((Window_Imp*)window)->GetWindow();

	glfwMakeContextCurrent(m_window);

	glewInit();

	m_renderState = new RenderState_Imp_GL(this);

	// フレームバッファ生成
	glGenFramebuffers(1, &m_frameBuffer);

	// バグ対策？
	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Graphics_Imp_GL::Graphics_Imp_GL(Vector2DI size, void* display, void* window, void* context, Log* log)
	: Graphics_Imp(size, log)
	, m_window(nullptr)
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
	glewInit();

	m_renderState = new RenderState_Imp_GL(this);

	// フレームバッファ生成
	glGenFramebuffers(1, &m_frameBuffer);

	// バグ対策？
	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
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

#if !_WIN32
	if( m_x11Mode )
	{
		glXMakeCurrent(m_x11Display, 0, NULL);
		glXDestroyContext(m_x11Display, m_glx);
	}
#endif
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
	// Save FPS
	ace::Sleep(16);

	if( m_window !=nullptr )
	{
		glfwSwapBuffers(m_window);
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
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

}
