#include "asd.EffekseerHelper.h"

#include "../asd.Graphics_Imp.h"

namespace asd
{
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	static void LoadPNG(const uint8_t* data, int32_t width, int32_t height, void* userData)
	{
		auto textureData = (Effekseer::TextureData*)(userData);

		GLuint texture = 0;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RGBA,
			width,
			height,
			0,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			data);

		// Generate mipmap
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);

		textureData->UserPtr = nullptr;
		textureData->UserID = texture;
		textureData->TextureFormat = Effekseer::TextureFormatType::ABGR8;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	class EffectTextureLoader_GL
		: public Effekseer::TextureLoader
	{
		Effekseer::FileInterface* fileInterface = nullptr;

	public:
		EffectTextureLoader_GL(Graphics_Imp* graphics, Effekseer::FileInterface* fileInterface)
			: fileInterface(fileInterface)
		{
		}
		virtual ~EffectTextureLoader_GL()
		{}

	public:
		Effekseer::TextureData* Load(const EFK_CHAR* path, Effekseer::TextureType textureType) override
		{
			std::unique_ptr<Effekseer::FileReader>
				reader(fileInterface->OpenRead(path));

			if (reader.get() != NULL)
			{
				size_t size_texture = reader->GetLength();
				char* data_texture = new char[size_texture];
				reader->Read(data_texture, size_texture);

				if (size_texture < 4)
				{
				}
				else if (data_texture[1] == 'P' &&
					data_texture[2] == 'N' &&
					data_texture[3] == 'G')
				{
					auto textureData = new Effekseer::TextureData();
					ar::ImageHelper::LoadPNG(LoadPNG, textureData, data_texture, size_texture);

					delete[] data_texture;

					return textureData;
				}
				else if (data_texture[0] == 'D' &&
					data_texture[1] == 'D' &&
					data_texture[2] == 'S' &&
					data_texture[3] == ' ')
				{
				}

				delete[] data_texture;
			}

			return nullptr;
		}

		void Unload(Effekseer::TextureData* data) override
		{
			if (data != NULL)
			{
				GLuint texture = data->UserID;
				glDeleteTextures(1, &texture);
			}

			if (data != nullptr)
			{
				delete data;
			}
		}
	};

	class EffectModelLoader_GL
		: public Effekseer::ModelLoader
	{
		Effekseer::FileInterface* fileInterface = nullptr;

	public:
		EffectModelLoader_GL(Graphics_Imp* graphics, Effekseer::FileInterface* fileInterface)
			: fileInterface(fileInterface)
		{
		}
		virtual ~EffectModelLoader_GL()
		{}

		void* Load(const EFK_CHAR* path)
		{
			std::unique_ptr<Effekseer::FileReader>
				reader(fileInterface->OpenRead(path));

			if (reader.get() != NULL)
			{
				size_t size_model = reader->GetLength();
				char* data_model = new char[size_model];
				reader->Read(data_model, size_model);

				::EffekseerRendererGL::Model* model = new ::EffekseerRendererGL::Model(data_model, size_model);

				delete[] data_model;

				return (void*)model;
			}
		}

		void Unload(void* data)
		{
			if (data != NULL)
			{
				::EffekseerRendererGL::Model* model = (::EffekseerRendererGL::Model*) data;
				delete model;
			}
		}
	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	static astring GetFileNameWithoutExtension(const achar* filepath)
	{
		auto path = astring(filepath);
		size_t i = path.rfind('.', path.length());
		if (i != astring::npos)
		{
			return (path.substr(0, i));
		}
		return astring();
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	class EffekseerFileReader :
		public Effekseer::FileReader
	{
		std::vector<uint8_t>	data;
		int32_t					position;
	public:
		EffekseerFileReader(const std::vector<uint8_t>& data)
		{
			this->data = data;
			position = 0;
		}

		virtual ~EffekseerFileReader() {}

		size_t Read(void* buffer, size_t size)
		{
			int32_t readable = size;
			if (data.size() - position < size) readable = data.size() - position;

			memcpy(buffer, &(data[position]), readable);
			position += readable;
			return readable;
		}

		void Seek(int position)
		{
			this->position = position;
			if (this->position < 0) this->position = 0;
			if (this->position > data.size()) this->position = data.size();
		}

		int GetPosition()
		{
			return position;
		}

		size_t GetLength()
		{
			return data.size();
		}
	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	EffekseerFile::EffekseerFile(Graphics_Imp* graphics)
		: graphics(graphics)
	{

	}

	EffekseerFile::~EffekseerFile() {}

	Effekseer::FileReader* EffekseerFile::OpenRead(const EFK_CHAR* path)
	{
		auto f = graphics->GetFile()->CreateStaticFile((const achar*)path);
		if (f != nullptr)
		{
			return new EffekseerFileReader(f->GetBuffer());
		}
		else
		{
			return nullptr;
		}
	}

	Effekseer::FileWriter* EffekseerFile::OpenWrite(const EFK_CHAR* path)
	{
		return nullptr;
	}
	
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	EffectTextureLoader::EffectTextureLoader(Graphics_Imp* graphics, Effekseer::TextureLoader* originalTextureLoader)
		: m_graphics(graphics)
		, originalTextureLoader(originalTextureLoader)
	{
	}

	EffectTextureLoader::~EffectTextureLoader()
	{
		assert(m_caches.size() == 0);
		SafeDelete(originalTextureLoader);
	}

	Effekseer::TextureData* EffectTextureLoader::Load(const EFK_CHAR* path, Effekseer::TextureType textureType)
	{
		auto key = astring((const achar*)path);
		auto cache = m_caches.find(key);
		if (cache != m_caches.end())
		{
			cache->second.Count++;
			return cache->second.Ptr;
		}

		auto nameWE = GetFileNameWithoutExtension((const achar*)path);

		// DDS優先読み込み
		while (true)
		{
			auto t = originalTextureLoader->Load((EFK_CHAR*)(nameWE + ToAString(".dds")).c_str(), textureType);
			if (t != nullptr)
			{
				Cache c;
				c.IsDDS = true;
				c.Ptr = t;
				c.Count = 1;
				c.Width = 0;
				c.Height = 0;
				m_caches[key] = c;
				dataToKey[t] = key;

				TextureFormat f = TextureFormat::R8G8B8A8_UNORM;
				if (t->TextureFormat == Effekseer::TextureFormatType::BC1) f = TextureFormat::BC1;
				if (t->TextureFormat == Effekseer::TextureFormatType::BC2) f = TextureFormat::BC2;
				if (t->TextureFormat == Effekseer::TextureFormatType::BC3) f = TextureFormat::BC3;

				m_graphics->IncVRAM(ImageHelper::GetVRAMSize(f, t->Width, t->Height));
			}
			else
			{
				break;
			}

			return t;
		}

		// PNG
		{
			auto t = originalTextureLoader->Load((EFK_CHAR*)path, textureType);
			if (t != nullptr)
			{
				Cache c;
				c.IsDDS = true;
				c.Ptr = t;
				c.Count = 1;
				c.Width = 0;
				c.Height = 0;
				m_caches[key] = c;
				dataToKey[t] = key;

				TextureFormat f = TextureFormat::R8G8B8A8_UNORM;
				if (t->TextureFormat == Effekseer::TextureFormatType::BC1) f = TextureFormat::BC1;
				if (t->TextureFormat == Effekseer::TextureFormatType::BC2) f = TextureFormat::BC2;
				if (t->TextureFormat == Effekseer::TextureFormatType::BC3) f = TextureFormat::BC3;

				m_graphics->IncVRAM(ImageHelper::GetVRAMSize(f, t->Width, t->Height));
			}

			return t;
		}
	}

	void EffectTextureLoader::Unload(Effekseer::TextureData* data)
	{
		if (data == nullptr) return;

		auto key = dataToKey[data];
		auto cache = m_caches.find(key);
		cache->second.Count--;

		if (cache->second.Count == 0)
		{
			TextureFormat f = TextureFormat::R8G8B8A8_UNORM;
			if (data->TextureFormat == Effekseer::TextureFormatType::BC1) f = TextureFormat::BC1;
			if (data->TextureFormat == Effekseer::TextureFormatType::BC2) f = TextureFormat::BC2;
			if (data->TextureFormat == Effekseer::TextureFormatType::BC3) f = TextureFormat::BC3;

			m_graphics->DecVRAM(ImageHelper::GetVRAMSize(f, cache->second.Width, cache->second.Height));
			m_caches.erase(key);
			dataToKey.erase(data);

			originalTextureLoader->Unload(data);
		}
	}


	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	EffectModelLoader::EffectModelLoader(Graphics_Imp* graphics, Effekseer::ModelLoader* loader)
		: m_graphics(graphics)
		, originalLoader(loader)
	{

	}

	EffectModelLoader::~EffectModelLoader()
	{
		assert(m_caches.size() == 0);
		SafeDelete(originalLoader);
	}

	void* EffectModelLoader::Load(const EFK_CHAR* path)
	{
		auto key = astring((const achar*)path);
		auto cache = m_caches.find(key);
		if (cache != m_caches.end())
		{
			cache->second.Count++;
			return cache->second.Ptr;
		}

		auto p = originalLoader->Load(path);
		if (p == nullptr) return nullptr;

		Cache c;
		c.Ptr = p;
		c.Count = 1;
		m_caches[key] = c;
		dataToKey[p] = key;

		return p;
	}

	void EffectModelLoader::Unload(void* data)
	{
		if (data == nullptr) return;

		auto key = dataToKey[data];
		auto cache = m_caches.find(key);
		cache->second.Count--;

		if (cache->second.Count == 0)
		{
			originalLoader->Unload(data);

			m_caches.erase(key);
			dataToKey.erase(data);
		}
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
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

		bool OnDistorting() override;
	};

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

	bool DistortingCallbackDX11::OnDistorting()
	{
		if (!IsEnabled)
		{
			renderer->SetBackground(nullptr);
			return false;
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

		return true;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
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
		bool OnDistorting() override;
	};

	DistortingCallbackGL::DistortingCallbackGL(EffekseerRendererGL::Renderer* renderer)
	{
		this->renderer = renderer;
		glGenTextures(1, &backGroundTexture);
#ifndef _WIN32
		glGenFramebuffers(1, &framebufferForCopy);
#endif
	}

	DistortingCallbackGL::~DistortingCallbackGL()
	{
		ReleaseTexture();
	}

	void DistortingCallbackGL::ReleaseTexture()
	{
#ifndef _WIN32
		glDeleteFramebuffers(1, &framebufferForCopy);
#endif
		glDeleteTextures(1, &backGroundTexture);
	}

	// コピー先のテクスチャを準備
	void DistortingCallbackGL::PrepareTexture(uint32_t width, uint32_t height, GLint internalFormat)
	{
		glBindTexture(GL_TEXTURE_2D, backGroundTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

		backGroundTextureWidth = width;
		backGroundTextureHeight = height;
		backGroundTextureInternalFormat = internalFormat;
	}

	bool DistortingCallbackGL::OnDistorting()
	{
		if (!IsEnabled)
		{
			renderer->SetBackground(0);
			return false;
		}

		GLint viewport[4];
		glGetIntegerv(GL_VIEWPORT, viewport);
		uint32_t width = viewport[2];
		uint32_t height = viewport[3];

		if (backGroundTextureWidth != width ||
			backGroundTextureHeight != height)
		{
			PrepareTexture(width, height, GL_RGBA);
		}

#ifndef _WIN32
		GLint backupFramebuffer;
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &backupFramebuffer);

		GLint rbtype;
		glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE, &rbtype);

		if (rbtype == GL_RENDERBUFFER) {
			GLint renderbuffer;
			glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
				GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME, &renderbuffer);

			glBindFramebuffer(GL_FRAMEBUFFER, framebufferForCopy);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, renderbuffer);
		}
		else if (rbtype == GL_TEXTURE_2D) {
			GLint renderTexture;
			glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
				GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME, &renderTexture);

			glBindFramebuffer(GL_FRAMEBUFFER, framebufferForCopy);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTexture, 0);
		}
#endif

		glBindTexture(GL_TEXTURE_2D, backGroundTexture);
		//glCopyTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, 0, 0, width, height );
		glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, viewport[0], viewport[1], width, height);
		glBindTexture(GL_TEXTURE_2D, 0);

#ifndef _WIN32
		glBindFramebuffer(GL_FRAMEBUFFER, backupFramebuffer);
#endif

		renderer->SetBackground(backGroundTexture);

		return true;
	}
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------

	static void* _AllocFunc(int32_t size, int32_t alignment, void* userData)
	{
		auto m = (ar::Manager*)userData;
		return m->Alloc(size, alignment);
	}

	static void _FreeFunc(void* data, void* userData)
	{
		auto m = (ar::Manager*)userData;
		m->Free((uint8_t*)data);
	}

	EffekseerRenderer::Renderer* EffekseerHelper::CreateRenderer(Graphics* graphics, int32_t spriteCount)
	{
		auto g = (Graphics_Imp*)graphics;
		EffekseerRenderer::Renderer* r = nullptr;

#if defined(_PSVITA)
		{
			auto rn = ::EffekseerRendererPSVita::Renderer::Create(
				g->GetRHI()->GetInternalObjects()[0],
				g->GetRHI()->GetInternalObjects()[1],
				_AllocFunc,
				_FreeFunc,
				g->GetRHI(),
				spriteCount);
			r = rn;
		}
#elif defined(_PS4)
		{
			auto rn = ::EffekseerRendererPS4::Renderer::Create(
				g->GetRHI()->GetInternalObjects()[0],
				g->GetRHI()->GetInternalObjects()[1],
				_AllocFunc,
				_FreeFunc,
				g->GetRHI(),
				spriteCount);
			r = rn;
		}
#elif defined(_SWITCH)
		{
			auto rn = ::EffekseerRendererSwitch::Renderer::Create(
				g->GetRHI()->GetInternalObjects()[0],
				g->GetRHI()->GetInternalObjects()[1],
				_AllocFunc,
				_FreeFunc,
				g->GetRHI(),
				spriteCount);
			r = rn;
		}
#elif defined(_XBOXONE)
		{
			auto rn = ::EffekseerRendererXBoxOne::Renderer::Create(
				g->GetRHI()->GetInternalObjects()[0],
				g->GetRHI()->GetInternalObjects()[1],
				_AllocFunc,
				_FreeFunc,
				g->GetRHI(),
				spriteCount);
			r = rn;
		}
#else

#if _WIN32
		if (g->GetGraphicsDeviceType() == GraphicsDeviceType::DirectX11)
		{
			auto rn = ::EffekseerRendererDX11::Renderer::Create(
				(ID3D11Device*)g->GetRHI()->GetInternalObjects()[0],
				(ID3D11DeviceContext*)g->GetRHI()->GetInternalObjects()[1],
				spriteCount);

			auto distortion = new DistortingCallbackDX11(
				rn,
				(ID3D11Device*)g->GetRHI()->GetInternalObjects()[0],
				(ID3D11DeviceContext*)g->GetRHI()->GetInternalObjects()[1]);

			rn->SetDistortingCallback(distortion);
			
			r = rn;
		}
#endif

		if (g->GetGraphicsDeviceType() == GraphicsDeviceType::OpenGL)
		{
			auto rn = ::EffekseerRendererGL::Renderer::Create(spriteCount, EffekseerRendererGL::OpenGLDeviceType::OpenGL3);

			auto distortion = new DistortingCallbackGL(
					rn);

			rn->SetDistortingCallback(distortion);

			r = rn;
		}
#endif

		return r;
	}

	Effekseer::TextureLoader* EffekseerHelper::CreateTextureLoader(Graphics* graphics, Effekseer::FileInterface* fileInterface)
	{
		auto g = (Graphics_Imp*)graphics;

#if defined(_PSVITA)

#elif defined(_PS4)

#elif defined(_SWITCH)

#elif defined(_XBOXONE)

#else

#if _WIN32
		if (g->GetGraphicsDeviceType() == GraphicsDeviceType::DirectX11)
		{
			return new EffectTextureLoader(
				g,
				EffekseerRendererDX11::CreateTextureLoader(
					(ID3D11Device*)g->GetRHI()->GetInternalObjects()[0],
					fileInterface));
		}

#endif
		if (g->GetGraphicsDeviceType() == GraphicsDeviceType::OpenGL)
		{
			return new EffectTextureLoader(
				g,
				new EffectTextureLoader_GL(
					g,
					fileInterface));
		}


#endif
		return nullptr;
	}

	Effekseer::ModelLoader* EffekseerHelper::CreateModelLoader(Graphics* graphics, Effekseer::FileInterface* fileInterface)
	{
		auto g = (Graphics_Imp*)graphics;

#if defined(_PSVITA)

#elif defined(_PS4)

#elif defined(_SWITCH)

#elif defined(_XBOXONE)

#else

#if _WIN32
		if (g->GetGraphicsDeviceType() == GraphicsDeviceType::DirectX11)
		{
			return new EffectModelLoader(
				g,
				EffekseerRendererDX11::CreateModelLoader(
				(ID3D11Device*)g->GetRHI()->GetInternalObjects()[0],
					fileInterface));
		}
#endif

		if (g->GetGraphicsDeviceType() == GraphicsDeviceType::OpenGL)
		{
			return new EffectModelLoader(
				g,
				new EffectModelLoader_GL(
					g,
					fileInterface));
		}

#endif
		return nullptr;
	}
}