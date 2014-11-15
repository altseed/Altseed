
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "ace.Graphics_Imp_GL.h"

#include "../../ace.RenderingThread.h"

#include "../../../Window/ace.Window_Imp.h"
#include "Resource/ace.VertexBuffer_Imp_GL.h"
#include "Resource/ace.IndexBuffer_Imp_GL.h"
#include "Resource/ace.NativeShader_Imp_GL.h"
#include "Resource/ace.DepthBuffer_Imp_GL.h"

#include "Resource/ace.Texture2D_Imp_GL.h"
#include "Resource/ace.RenderTexture2D_Imp_GL.h"
#include "Resource/ace.CubemapTexture_Imp_GL.h"

#include "../../../Log/ace.Log.h"

// Windows以外でGoogleTestとGLFWNativeのヘッダが干渉する(2013/12)
#include <GLFW/glfw3native.h>

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void GraphicsHelper_GL::LoadTexture(Graphics_Imp_GL* graphics, void* imgdata, int32_t width, int32_t height, GLuint& texture)
	{
		texture = 0;

		glGenTextures(1, &texture);

		GLCheckError();
		if (glGetError() != GL_NO_ERROR) return;

		glBindTexture(GL_TEXTURE_2D, texture);

		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_SRGB8_ALPHA8,
			width,
			height,
			0,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			imgdata);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		
		// ミップマップ生成しないと正しく描画されない
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);

		if (glGetError() != GL_NO_ERROR)
		{
			glDeleteTextures(1, &texture);
			texture = 0;
			return;
		}
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	class EffectTextureLoader_GL
		: public EffectTextureLoader
	{
	public:
		EffectTextureLoader_GL(Graphics_Imp_GL* graphics)
			:EffectTextureLoader(graphics)
		{
		}
		virtual ~EffectTextureLoader_GL()
		{}

	public:
		void* InternalLoad(Graphics_Imp* graphics, std::vector<uint8_t>& data, int32_t width, int32_t height)
		{
			GLuint texture = 0;
			GraphicsHelper_GL::LoadTexture((Graphics_Imp_GL*) m_graphics, data.data(), width, height, texture);

			return (void*) texture;
		}

		void InternalUnload(void* data)
		{
			if (data != NULL)
			{
				uint64_t data_ = reinterpret_cast<uint64_t>(data);
				GLuint texture = static_cast<GLuint>(data_);
				glDeleteTextures(1, &texture);
			}
		}

		bool IsReversed()
		{
			return true;
		}
	};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
	Graphics_Imp_GL::Graphics_Imp_GL(Vector2DI size, ::ace::Window* window, Log* log, bool isReloadingEnabled, bool isFullScreen)
		: Graphics_Imp(size, log, isReloadingEnabled, isFullScreen)
	, m_window(window)
	, m_endStarting(false)
{
	assert(window != nullptr);
	SafeAddRef(m_window);


	auto window_ = ((Window_Imp*)window)->GetWindow();

	glfwMakeContextCurrent(window_);
	GLCheckError();
	
	glEnable(GL_FRAMEBUFFER_SRGB);

	// 同期しない
	glfwSwapInterval(0);
	GLCheckError();

#pragma region RenderState
	glGenSamplers(MaxTextureCount, m_samplers);
	GLCheckError();
#pragma endregion
	
	// フレームバッファ生成
	glGenFramebuffers(1, &m_frameBuffer);
	GLCheckError();

	// バグ対策？
	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	GLCheckError();

#ifdef __APPLE__
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	GLCheckError();
#endif

	// スレッド生成
	MakeContextNone();

#ifdef __APPLE__
	GLCheckError();
#endif

	CreateContextBeforeThreading(window_);

#ifdef __APPLE__
	GLCheckError();
#endif

	m_renderingThread->Run(this, StartRenderingThreadFunc, EndRenderingThreadFunc);
	while (!m_endStarting)
	{
		Sleep(1);
	}
	CreateContextAfterThreading(window_);

#ifdef __APPLE__
	GLCheckError();
#endif

	MakeContextCurrent();
	WriteInitializedLog(m_log);
	GLCheckError();

	GetEffectSetting()->SetTextureLoader(new EffectTextureLoader_GL(this));
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
	Graphics_Imp_GL::Graphics_Imp_GL(Vector2DI size, void* display, void* window, void* context, Log* log, bool isReloadingEnabled, bool isFullScreen)
		: Graphics_Imp(size, log, isReloadingEnabled, isFullScreen)
	, m_window(nullptr)
	, m_endStarting(false)
{
#if _WIN32
#elif __APPLE__
#else
	GLXContext* context_ = (GLXContext*)context;
	m_glx = *context_;

	Display* display_ = (Display*)display;
	::Window window_ = *((::Window*)window);
	
	m_x11Display = display_;
	m_x11Window = window_;

	glXMakeCurrent(m_x11Display, m_x11Window, m_glx);
	GLCheckError();

	m_x11Mode = true;
#endif

#pragma region RenderState
	glGenSamplers(MaxTextureCount, m_samplers);
#pragma endregion
	GLCheckError();

	// フレームバッファ生成
	glGenFramebuffers(1, &m_frameBuffer);
	GLCheckError();

	// バグ対策？
	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	GLCheckError();

#ifdef __APPLE__
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	GLCheckError();
#endif

	// スレッド生成
	MakeContextNone();
	GLCheckError();

	glEnable(GL_FRAMEBUFFER_SRGB);

	CreateContextBeforeThreading(nullptr);
	GLCheckError();

	m_renderingThread->Run(this, StartRenderingThreadFunc, EndRenderingThreadFunc);
	while (!m_endStarting)
	{
		Sleep(1);
	}
	CreateContextAfterThreading(nullptr);
	GLCheckError();
	


	MakeContextCurrent();
	WriteInitializedLog(m_log);
	GLCheckError();

	GetEffectSetting()->SetTextureLoader(new EffectTextureLoader_GL(this));
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Graphics_Imp_GL::~Graphics_Imp_GL()
{
	// 必要なし
	//SafeRelease(m_currentShader);

	m_renderingThread->AddEvent(nullptr);
	while (m_renderingThread->IsRunning())
	{
		Sleep(1);
	}
	m_renderingThread.reset();
	

#pragma region RenderState
	glDeleteSamplers(MaxTextureCount, m_samplers);
#pragma endregion

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDeleteFramebuffers(1, &m_frameBuffer);

#ifdef __APPLE__
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &m_vao);
	GLCheckError();
#endif

#if _WIN32
#elif __APPLE__
#else
	if( m_x11Mode )
	{
		glXMakeCurrent(m_x11Display, 0, NULL);
		glXDestroyContext(m_x11Display, m_glx);
	}
#endif

	SafeRelease(m_window);
}

void Graphics_Imp_GL::WriteInitializedLog(Log* log)
{
	if(log == nullptr) return;

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

	writeLogHeading(ToAString("OpenGL"));
	writeLog(ToAString(""));

	WriteDeviceInformation(log);
}

void Graphics_Imp_GL::WriteDeviceInformation(Log* log)
{
	log->WriteLineStrongly(ToAString("ビデオカード情報").c_str());

	log->BeginTable();

	log->Write(ToAString("バージョン").c_str());
	log->ChangeColumn();

#ifndef __APPLE__
	if (GLEW_VERSION_4_0) { log->Write(ToAString("4.0").c_str()); }
	else if (GLEW_VERSION_3_3) { log->Write(ToAString("3.3").c_str()); }
	else if (GLEW_VERSION_3_2) { log->Write(ToAString("3.2").c_str()); }
	else if (GLEW_VERSION_3_1) { log->Write(ToAString("3.1").c_str()); }
	else if (GLEW_VERSION_3_0) { log->Write(ToAString("3.0").c_str()); }
	else if (GLEW_VERSION_2_1) { log->Write(ToAString("2.1").c_str()); }
	else  { log->Write(ToAString("Unknown").c_str()); }
#endif
	log->EndTable();
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
void Graphics_Imp_GL::UpdateStatus(VertexBuffer_Imp* vertexBuffer, IndexBuffer_Imp* indexBuffer, NativeShader_Imp* shaderPtr)
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
		GLCheckError();

		// レイアウトの設定
		shader->SetLayout();
		GLCheckError();

		// 定数バッファの設定
		shader->AssignConstantBuffer();
		GLCheckError();

		// テクスチャの設定
		for (int32_t i = 0; i < Graphics_Imp::MaxTextureCount; i++)
		{
			Texture* tex = nullptr;
			char* texName = nullptr;
			TextureFilterType filterType;
			TextureWrapType wrapType;

			if (shader->GetTexture(texName, tex, filterType, wrapType, i))
			{
				GLuint buf = 0;
				if (tex->GetType() == TEXTURE_CLASS_TEXTURE2D)
				{
					auto t = (Texture2D_Imp_GL*) tex;
					buf = t->GetBuffer();
				}
				else if (tex->GetType() == TEXTURE_CLASS_RENDERTEXTURE2D)
				{
					auto t = (RenderTexture2D_Imp_GL*) tex;
					buf = t->GetBuffer();
				}
				else if (tex->GetType() == TEXTURE_CLASS_CUBEMAPTEXTURE)
				{
					auto t = (CubemapTexture_Imp_GL*) tex;
					buf = t->GetBuffer();
				}
				else if (tex->GetType() == TEXTURE_CLASS_DEPTHBUFFER)
				{
					auto t = (DepthBuffer_Imp_GL*) tex;
					buf = t->GetBuffer();
				}

				GLCheckError();
				glActiveTexture(GL_TEXTURE0 + i);

				if (tex->GetType() == TEXTURE_CLASS_CUBEMAPTEXTURE)
				{
					glBindTexture(GL_TEXTURE_CUBE_MAP, buf);
				}
				else
				{
					glBindTexture(GL_TEXTURE_2D, buf);
				}
				
				GLCheckError();

				auto id = glGetUniformLocation(program, texName);
				glUniform1i(id, i);
				GLCheckError();

				// ステート設定
				nextState.textureFilterTypes[i] = filterType;
				nextState.textureWrapTypes[i] = wrapType;
			}
		}

		GLCheckError();
	}

	CommitRenderState(false);

	// MIPMAPの処理(shader依存のためCommitRenderStateでは不可)
	static const GLint glfilter [] = { GL_NEAREST, GL_LINEAR };
	static const GLint glfilter_mip [] = { GL_NEAREST_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_LINEAR };

	static const GLint glwrap [] = { GL_REPEAT, GL_CLAMP_TO_EDGE };
	
	auto& stateFilter = currentState.textureFilterTypes;
	auto& stateWrap = currentState.textureWrapTypes;

	for (int32_t i = 0; i < MaxTextureCount; i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindSampler(i, m_samplers[i]);

		int32_t filter_ = (int32_t) stateFilter[i];

		Texture* tex = nullptr;
		char* texName = nullptr;
		TextureFilterType filterType;
		TextureWrapType wrapType;

		if (shader->GetTexture(texName, tex, filterType, wrapType, i))
		{
			if (tex->GetType() == TEXTURE_CLASS_CUBEMAPTEXTURE)
			{
				glSamplerParameteri(m_samplers[i], GL_TEXTURE_MAG_FILTER, glfilter[filter_]);
				glSamplerParameteri(m_samplers[i], GL_TEXTURE_MIN_FILTER, glfilter_mip[filter_]);
			}
			else
			{
				glSamplerParameteri(m_samplers[i], GL_TEXTURE_MAG_FILTER, glfilter[filter_]);
				glSamplerParameteri(m_samplers[i], GL_TEXTURE_MIN_FILTER, glfilter[filter_]);
			}

		}

		glActiveTexture(GL_TEXTURE0 + i);

		glBindSampler(i, m_samplers[i]);
		int32_t wrap_ = (int32_t) stateWrap[i];
		glSamplerParameteri(m_samplers[i], GL_TEXTURE_WRAP_S, glwrap[wrap_]);
		glSamplerParameteri(m_samplers[i], GL_TEXTURE_WRAP_T, glwrap[wrap_]);
	}

	glActiveTexture(GL_TEXTURE0);
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

	UpdateStatus(vb, ib, shader);
	GLCheckError();

	if (indexBuffer->Is32Bit())
	{
		glDrawElements(GL_TRIANGLES, count * 3, GL_UNSIGNED_INT, NULL);
	}
	else
	{
		glDrawElements(GL_TRIANGLES, count * 3, GL_UNSIGNED_SHORT, NULL);
	}
	GLCheckError();

	{
		auto shader = (NativeShader_Imp_GL*) shaderPtr;
		shader->Disable();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	GLCheckError();
}

void Graphics_Imp_GL::DrawPolygonInternal(int32_t offset, int32_t count, VertexBuffer_Imp* vertexBuffer, IndexBuffer_Imp* indexBuffer, NativeShader_Imp* shaderPtr)
{
	assert(vertexBuffer != nullptr);
	assert(indexBuffer != nullptr);
	assert(shaderPtr != nullptr);

	auto shader = (NativeShader_Imp_GL*) shaderPtr;
	auto vb = (VertexBuffer_Imp_GL*) vertexBuffer;
	auto ib = (IndexBuffer_Imp_GL*) indexBuffer;

	UpdateStatus(vb, ib, shader);
	GLCheckError();

	if (indexBuffer->Is32Bit())
	{
		glDrawElements(GL_TRIANGLES, count * 3, GL_UNSIGNED_INT, (void*) (offset * 3 * sizeof(uint32_t)));
	}
	else
	{
		glDrawElements(GL_TRIANGLES, count * 3, GL_UNSIGNED_SHORT, (void*) (offset * 3 * sizeof(uint16_t)));
	}
	GLCheckError();

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
void Graphics_Imp_GL::DrawPolygonInstancedInternal(int32_t count, VertexBuffer_Imp* vertexBuffer, IndexBuffer_Imp* indexBuffer, NativeShader_Imp* shaderPtr, int32_t instanceCount)
{
	assert(vertexBuffer != nullptr);
	assert(indexBuffer != nullptr);
	assert(shaderPtr != nullptr);


	auto shader = (NativeShader_Imp_GL*) shaderPtr;
	auto vb = (VertexBuffer_Imp_GL*) vertexBuffer;
	auto ib = (IndexBuffer_Imp_GL*) indexBuffer;

	UpdateStatus(vb, ib, shader);

	if (indexBuffer->Is32Bit())
	{
		glDrawElementsInstanced(GL_TRIANGLES, count * 3, GL_UNSIGNED_INT, NULL, instanceCount);
	}
	else
	{
		glDrawElementsInstanced(GL_TRIANGLES, count * 3, GL_UNSIGNED_SHORT, NULL, instanceCount);
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
void Graphics_Imp_GL::EndInternal()
{
	GLCheckError();
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Graphics_Imp_GL::SetViewport(int32_t x, int32_t y, int32_t width, int32_t height)
{
	MakeContextCurrent();

	glViewport(x, y, width, height);
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Graphics_Imp_GL* Graphics_Imp_GL::Create(::ace::Window* window, Log* log, bool isReloadingEnabled, bool isFullScreen)
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

	writeLogHeading(ToAString("OpenGL"));

	auto window_ = ((Window_Imp*) window)->GetWindow();
	glfwMakeContextCurrent(window_);
	GLCheckError();

#ifndef __APPLE__
	if (glewInit() != GLEW_OK)
	{
		writeLog(ToAString("GLEWの初期化に失敗"));
		goto End;
	}

	else if (!GLEW_VERSION_3_3)
	{
		writeLog(ToAString("OpenGL3.3に未対応"));
		goto End;
	}

	GLCheckError();
#endif

	writeLog(ToAString("OpenGL初期化成功"));
	writeLog(ToAString(""));

	return new Graphics_Imp_GL(window->GetSize(), window, log, isReloadingEnabled, isFullScreen);

End:;
	writeLog(ToAString("OpenGL初期化失敗"));
	writeLog(ToAString(""));
	return nullptr;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#if _WIN32
#elif __APPLE__
#else
Graphics_Imp_GL* Graphics_Imp_GL::Create_X11(void* display, void* window, int32_t width, int32_t height, Log* log, bool isReloadingEnabled, bool isFullScreen )
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

	writeLogHeading(ToAString("OpenGL"));

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

	glXMakeCurrent(display_, window_, context);

	if (glewInit() != GLEW_OK)
	{
		writeLog(ToAString("GLEWの初期化に失敗"));
		goto End;
	}

	else if (!GLEW_VERSION_3_3)
	{
		writeLog(ToAString("OpenGL3.3に未対応"));
		goto End;
	}

	writeLog(ToAString("OpenGL初期化成功"));
	writeLog(ToAString(""));

	return new Graphics_Imp_GL( ace::Vector2DI(width,height), display, window, context_, log, isReloadingEnabled);

End:;
	writeLog(ToAString("OpenGL初期化失敗"));
	writeLog(ToAString(""));
	return nullptr;
}
#endif

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Texture2D_Imp* Graphics_Imp_GL::CreateTexture2D_Imp_Internal(Graphics* graphics, uint8_t* data, int32_t size)
{
	auto ret = Texture2D_Imp_GL::Create(this, data, size, true);
	return ret;
}

Texture2D_Imp* Graphics_Imp_GL::CreateTexture2DAsRawData_Imp_Internal(Graphics* graphics, uint8_t* data, int32_t size)
{
	auto ret = Texture2D_Imp_GL::Create(this, data, size, false);
	return ret;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Texture2D_Imp* Graphics_Imp_GL::CreateEmptyTexture2D_Imp_Internal(Graphics* graphics, int32_t width, int32_t height, TextureFormat format)
{
	auto ret = Texture2D_Imp_GL::Create(this, width, height, format);
	return ret;
}

CubemapTexture* Graphics_Imp_GL::CreateCubemapTextureFrom6ImageFiles_(const achar* front, const achar* left, const achar* back, const achar* right, const achar* top, const achar* bottom)
{
	return CubemapTexture_Imp_GL::Create(this, front, left, back, right, top, bottom);
}

CubemapTexture* Graphics_Imp_GL::CreateCubemapTextureFromMipmapImageFiles_(const achar* path, int32_t mipmapCount)
{
	return CubemapTexture_Imp_GL::Create(this, path, mipmapCount);
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
RenderTexture2D_Imp* Graphics_Imp_GL::CreateRenderTexture2D_Imp(int32_t width, int32_t height, TextureFormat format)
{
	return RenderTexture2D_Imp_GL::Create(this, width, height, format);
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
DepthBuffer_Imp* Graphics_Imp_GL::CreateDepthBuffer_Imp(int32_t width, int32_t height)
{
	return DepthBuffer_Imp_GL::Create(this, width, height);
}

void Graphics_Imp_GL::CommitRenderState(bool forced)
{
	GLCheckError();

	auto& current = currentState.renderState;
	auto& next = nextState.renderState;

	auto& currentFilter = currentState.textureFilterTypes;
	auto& nextFilter = nextState.textureFilterTypes;

	auto& currentWrap = currentState.textureWrapTypes;
	auto& nextWrap = nextState.textureWrapTypes;

	if (current.DepthTest != next.DepthTest || forced)
	{
		if (next.DepthTest)
		{
			glEnable(GL_DEPTH_TEST);
		}
		else
		{
			glDisable(GL_DEPTH_TEST);
		}
	}

	if (current.DepthWrite != next.DepthWrite || forced)
	{
		glDepthMask(next.DepthWrite);
		glDepthFunc(GL_LEQUAL);
	}

	if (current.Culling != next.Culling || forced)
	{
		if (next.Culling == CullingType::Front)
		{
			glEnable(GL_CULL_FACE);
			glCullFace(GL_FRONT);
		}
		else if (next.Culling == CullingType::Back)
		{
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
		}
		else if (next.Culling == CullingType::Double)
		{
			glDisable(GL_CULL_FACE);
			glCullFace(GL_FRONT_AND_BACK);
		}
	}

	if (current.AlphaBlendState != next.AlphaBlendState || forced)
	{
		if (next.AlphaBlendState == AlphaBlend::Opacity)
		{
			glDisable(GL_BLEND);
		}
		else
		{
			glEnable(GL_BLEND);

			if (next.AlphaBlendState == AlphaBlend::Sub)
			{
				//glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);
				//glBlendFunc(GL_SRC_ALPHA, GL_ONE);
				glBlendEquationSeparate(GL_FUNC_REVERSE_SUBTRACT, GL_FUNC_ADD);
				glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE, GL_ONE, GL_ONE);
			}
			else
			{
				//glBlendEquation(GL_FUNC_ADD);
				glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
				if (next.AlphaBlendState == AlphaBlend::Blend)
				{
					glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);
				}
				else if (next.AlphaBlendState == AlphaBlend::Add)
				{
					glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE, GL_ONE, GL_ONE);
				}
				else if (next.AlphaBlendState == AlphaBlend::Mul)
				{
					glBlendFuncSeparate(GL_ZERO, GL_SRC_COLOR, GL_ONE, GL_ONE);
				}
			}
		}
	}

	currentState = nextState;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Graphics_Imp_GL::SetRenderTarget(RenderTexture2D_Imp* texture, DepthBuffer_Imp* depthBuffer)
{
	// 強制リセット
	ResetDrawState();
	for (int32_t i = 0; i < Graphics_Imp::MaxTextureCount; i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	glActiveTexture(GL_TEXTURE0);
	GLCheckError();

	if (texture == nullptr)
	{
		UnbindFramebuffer();
		glDrawBuffer(GL_BACK);
		CommitRenderState(true);
		SetViewport(0, 0, m_size.X, m_size.Y);
		GLCheckError();
		return;
	}

	GLuint cb = 0;
	GLuint db = 0;

	if (texture != nullptr)
	{
		cb = ((RenderTexture2D_Imp_GL*) texture)->GetBuffer();
	}

	if (depthBuffer != nullptr)
	{
		db = ((DepthBuffer_Imp_GL*) depthBuffer)->GetBuffer();
	}

	BindFramebuffer();
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, cb, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, 0, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, 0, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, 0, 0);
	GLCheckError();

	if (depthBuffer != nullptr)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, db, 0);
	}
	else
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, 0, 0);
	}
	GLCheckError();

	static const GLenum bufs [] = {
		GL_COLOR_ATTACHMENT0,
	};
	glDrawBuffers(1, bufs);
	GLCheckError();

	CommitRenderState(true);
	GLCheckError();

	if (texture != nullptr)
	{
		SetViewport(0, 0, texture->GetSize().X, texture->GetSize().Y);
	}
	GLCheckError();
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Graphics_Imp_GL::SetRenderTarget(RenderTexture2D_Imp* texture1, RenderTexture2D_Imp* texture2, RenderTexture2D_Imp* texture3, RenderTexture2D_Imp* texture4, DepthBuffer_Imp* depthBuffer)
{
	// 強制リセット
	ResetDrawState();
	for (int32_t i = 0; i < Graphics_Imp::MaxTextureCount; i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	glActiveTexture(GL_TEXTURE0);
	GLCheckError();

	if (texture1 == nullptr)
	{
		UnbindFramebuffer();
		glDrawBuffer(GL_BACK);
		CommitRenderState(true);
		SetViewport(0, 0, m_size.X, m_size.Y);
		GLCheckError();
		return;
	}

	GLuint cb[MaxRenderTarget] = { 0, 0, 0, 0};
	GLuint db = 0;

	if (texture1 != nullptr)
	{
		cb[0] = ((RenderTexture2D_Imp_GL*) texture1)->GetBuffer();
	}

	if (texture2 != nullptr)
	{
		cb[1] = ((RenderTexture2D_Imp_GL*) texture2)->GetBuffer();
	}

	if (texture3 != nullptr)
	{
		cb[2] = ((RenderTexture2D_Imp_GL*) texture3)->GetBuffer();
	}

	if (texture4 != nullptr)
	{
		cb[3] = ((RenderTexture2D_Imp_GL*) texture4)->GetBuffer();
	}

	if (depthBuffer != nullptr)
	{
		db = ((DepthBuffer_Imp_GL*) depthBuffer)->GetBuffer();
	}

	BindFramebuffer();
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, cb[0], 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, cb[1], 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, cb[2], 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, cb[3], 0);
	GLCheckError();

	if (depthBuffer != nullptr)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, db, 0);
	}
	else
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, 0, 0);
	}
	GLCheckError();

	static const GLenum bufs[] = {
		GL_COLOR_ATTACHMENT0,
		GL_COLOR_ATTACHMENT1,
		GL_COLOR_ATTACHMENT2,
		GL_COLOR_ATTACHMENT3,
	};
	glDrawBuffers(4, bufs);
	GLCheckError();

	CommitRenderState(true);
	GLCheckError();

	if (texture1 != nullptr)
	{
		SetViewport(0, 0, texture1->GetSize().X, texture1->GetSize().Y);
	}
	GLCheckError();
}

void Graphics_Imp_GL::SetRenderTarget(CubemapTexture_Imp* texture, int32_t direction, int32_t mipmap, DepthBuffer_Imp* depthBuffer)
{
	auto tex = (CubemapTexture_Imp_GL*) texture;

	// 強制リセット
	ResetDrawState();
	for (int32_t i = 0; i < Graphics_Imp::MaxTextureCount; i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	glActiveTexture(GL_TEXTURE0);
	GLCheckError();

	if (texture == nullptr)
	{
		UnbindFramebuffer();
		glDrawBuffer(GL_BACK);
		CommitRenderState(true);
		SetViewport(0, 0, m_size.X, m_size.Y);
		GLCheckError();
		return;
	}

	GLuint cb = 0;
	GLuint db = 0;

	if (texture != nullptr)
	{
		cb = tex->GetBuffer();
	}

	if (depthBuffer != nullptr)
	{
		db = ((DepthBuffer_Imp_GL*) depthBuffer)->GetBuffer();
	}

	static const GLenum target [] = {
		GL_TEXTURE_CUBE_MAP_POSITIVE_X,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
	};

	BindFramebuffer();
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, target[direction], cb, mipmap);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, 0, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, 0, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, 0, 0);
	GLCheckError();

	if (depthBuffer != nullptr)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, db, 0);
	}
	else
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, 0, 0);
	}
	GLCheckError();

	static const GLenum bufs [] = {
		GL_COLOR_ATTACHMENT0,
	};
	glDrawBuffers(1, bufs);
	GLCheckError();

	CommitRenderState(true);
	GLCheckError();

	if (texture != nullptr)
	{
		SetViewport(0, 0, tex->GetSize().X >> mipmap, tex->GetSize().Y >> mipmap);
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
		CommitRenderState(true);
	}

	GLCheckError();
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Graphics_Imp_GL::Present()
{
	GLCheckError();

	if (m_window != nullptr)
	{
		auto window_ = ((Window_Imp*) m_window)->GetWindow();
		glfwSwapBuffers(window_);
	}
	else
	{
#if _WIN32
#elif __APPLE__
#else
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

	ImageHelper::SavePNGImage(path, m_size.X, m_size.Y, buf, true);

	SafeDeleteArray(buf);

	GLCheckError();
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
bool Graphics_Imp_GL::SaveTexture(const achar* path, GLuint texture, Vector2DI size)
{
	GLCheckError();

	auto buf = new uint8_t[size.X * size.Y * 4];

	glBindTexture(GL_TEXTURE_2D, texture);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, buf);

	glBindTexture(GL_TEXTURE_2D, 0);

	ImageHelper::SavePNGImage(path, size.X, size.Y, buf, true);

	SafeDeleteArray(buf);

	GLCheckError();

	return true;
}

void Graphics_Imp_GL::MakeContextCurrent()
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

	GLCheckError();	
}

void Graphics_Imp_GL::MakeContextNone()
{
	if (m_window != nullptr)
	{
		glfwMakeContextCurrent(nullptr);
	}
	else
	{
		assert(0);
	}
}

void Graphics_Imp_GL::FlushCommand()
{
	glFlush();
	glFinish();
}

void Graphics_Imp_GL::CreateContextBeforeThreading(GLFWwindow* window)
{
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

void Graphics_Imp_GL::BindFramebuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
	GLCheckError();
}

void Graphics_Imp_GL::UnbindFramebuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	GLCheckError();
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

}
