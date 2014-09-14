
#include "ace.Renderer3D.h"

#include "../../ace.Graphics_Imp.h"

#include "../Object/ace.RenderedObject3D.h"
#include "../Object/ace.RenderedCameraObject3D.h"
#include "../Object/ace.RenderedDirectionalLightObject3D.h"

#include "../../Command/ace.RenderingCommandExecutor.h"

#if _WIN32
#include "../../Platform/DX11/ace.Graphics_Imp_DX11.h"
#endif
#include "../../Platform/GL/ace.Graphics_Imp_GL.h"

#include "ace.Renderer3DProxy.h"

#include "../../Resource/ace.CubemapTexture.h"

namespace ace
{
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	template<typename T>
	void AddRefToObjects(std::set<T*>& os)
	{
		for (auto& o : os)
		{
			SafeAddRef(o);
		}
	}

	template<typename T>
	void ReleaseObjects(std::set<T*>& os)
	{
		for (auto& o : os)
		{
			o->Release();
		}
		os.clear();
	}

	template<typename T>
	void CallRemovingObjects(std::set<T*>& os, Renderer3D* renderer)
	{
		for (auto& o : os)
		{
			o->OnRemoving(renderer);
		}
	}

	Renderer3D::RenderingEvent::RenderingEvent(Renderer3D* renderer)
		: m_renderer(renderer)
	{
	
	}

	Renderer3D::RenderingEvent::~RenderingEvent()
	{
	
	}

	void Renderer3D::RenderingEvent::Event()
	{
		m_renderer->proxy->Rendering(m_renderer->GetRenderTarget());
	}

	Renderer3D::Renderer3D(Graphics* graphics, RenderSettings settings)
		: m_graphics(nullptr)
		, m_settings(settings)
		, m_renderTarget(nullptr)
		, m_event(this)
		, m_skyAmbientColor(Color(10,10,20,255))
		, m_groundAmbientColor(Color(10,10,10,255))
	{
		m_graphics = (Graphics_Imp*) graphics;
		SafeAddRef(m_graphics);

		// エフェクト
		{
			m_effectManager = ::Effekseer::Manager::Create(2000, false);
			if (m_graphics->GetGraphicsDeviceType() == GraphicsDeviceType::DirectX11)
			{
#if _WIN32
				auto g = (Graphics_Imp_DX11*) m_graphics;
				m_effectRenderer = ::EffekseerRendererDX11::Renderer::Create(g->GetDevice(), g->GetContext(), 2000);
#endif
			}
			else if (m_graphics->GetGraphicsDeviceType() == GraphicsDeviceType::OpenGL)
			{
				m_effectRenderer = ::EffekseerRendererGL::Renderer::Create(2000);
			}

			m_effectManager->SetSpriteRenderer(m_effectRenderer->CreateSpriteRenderer());
			m_effectManager->SetRibbonRenderer(m_effectRenderer->CreateRibbonRenderer());
			m_effectManager->SetRingRenderer(m_effectRenderer->CreateRingRenderer());
			m_effectManager->SetModelRenderer(m_effectRenderer->CreateModelRenderer());
			m_effectManager->SetTrackRenderer(m_effectRenderer->CreateTrackRenderer());

			m_effectManager->SetSetting(m_graphics->GetEffectSetting());
		}

		executor = new RenderingCommandExecutor();

		proxy = new Renderer3DProxy(graphics);
	}

	Renderer3D::~Renderer3D()
	{
		while (!m_event.IsExited())
		{
			Sleep(1);
		}
		
		CallRemovingObjects(objects, this);
		ReleaseObjects(objects);
		
		ReleaseObjects(newObjects);
		ReleaseObjects(removingObjects);

		SafeRelease(m_renderTarget);

		m_effectRenderer->Destory();
		m_effectManager->Destroy();
		m_effectRenderer = nullptr;
		m_effectManager = nullptr;

		SafeDelete(executor);

		SafeDelete(proxy);
		SafeRelease(m_graphics);
	}

	RenderSettings Renderer3D::GetRenderSettings() const
	{
		return m_settings;
	}

	void Renderer3D::SetRenderSettings(RenderSettings settings)
	{
		m_settings = settings;
	}

	void Renderer3D::SetWindowSize(Vector2DI windowSize)
	{
		SafeRelease(m_renderTarget);
		m_renderTarget = m_graphics->CreateRenderTexture2D_Imp(windowSize.X, windowSize.Y, TextureFormat::R8G8B8A8_UNORM);
		m_windowSize = windowSize;

		if (m_graphics->GetGraphicsDeviceType() == GraphicsDeviceType::DirectX11)
		{
			m_effectRenderer->SetProjectionMatrix(::Effekseer::Matrix44().PerspectiveFovRH(90.0f / 180.0f * 3.14f, windowSize.X / windowSize.Y, 1.0f, 50.0f));
		}
		else if (m_graphics->GetGraphicsDeviceType() == GraphicsDeviceType::OpenGL)
		{
			m_effectRenderer->SetProjectionMatrix(::Effekseer::Matrix44().PerspectiveFovRH_OpenGL(90.0f / 180.0f * 3.14f, windowSize.X / windowSize.Y, 1.0f, 50.0f));
		}
	}

	void Renderer3D::AddObject(RenderedObject3D* o)
	{
		if (objects.count(o) == 0)
		{
			SafeAddRef(o);
			objects.insert(o);
			o->OnAdded(this);

			if (removingObjects.count(o) > 0)
			{
				removingObjects.erase(o);
				SafeRelease(o);
			}
			else
			{
				newObjects.insert(o);
				SafeAddRef(o);
			}
		}
	}

	void Renderer3D::RemoveObject(RenderedObject3D* o)
	{
		if (objects.count(o) > 0)
		{
			o->OnRemoving(this);
			objects.erase(o);
			o->Release();

			if (newObjects.count(o) > 0)
			{
				newObjects.erase(o);
				SafeRelease(o);
			}
			else
			{
				removingObjects.insert(o);
				SafeAddRef(o);
			}
		}
	}

	void Renderer3D::Flip()
	{
		for (auto& o : newObjects)
		{
			proxy->AddObject(o);
		}

		for (auto& o : removingObjects)
		{
			proxy->RemoveObject(o);
		}

		ReleaseObjects(newObjects);
		ReleaseObjects(removingObjects);

		m_effectManager->Flip();

		for (auto& o : objects)
		{
			o->Flip();
		}

		proxy->SetEffect(m_effectManager, m_effectRenderer);
		proxy->SkyAmbientColor = m_skyAmbientColor;
		proxy->GroundAmbientColor = m_groundAmbientColor;
		proxy->EnvironmentDiffuseColor = environment_diffuseColor;
		proxy->EnvironmentSpecularColor = environment_specularColor;
		proxy->Settings = m_settings;

		proxy->SSAO_Radius = SSAO_Radius;
		proxy->SSAO_FarPlain = SSAO_FarPlain;
		proxy->SSAO_Intensity = SSAO_Intensity;
		proxy->SSAO_Bias = SSAO_Bias;
	}

	void Renderer3D::BeginRendering(float deltaTime)
	{
		assert(m_renderTarget != nullptr);

		proxy->DeltaTime = deltaTime;
		m_graphics->GetRenderingThread()->AddEvent(&m_event);
	
	}

	void Renderer3D::EndRendering()
	{
		while (!m_event.IsExited())
		{
			Sleep(1);
		}
		
		executor->Execute(m_graphics, m_effectManager, m_effectRenderer, proxy->GetCommands());
		proxy->ResetCommands();
	}

	void Renderer3D::SetEnvironmentColor(CubemapTexture* diffuseColor, CubemapTexture* specularColor)
	{
		SafeAddRef(diffuseColor);
		SafeAddRef(specularColor);

		environment_diffuseColor = CreateSharedPtrWithReleaseDLL(diffuseColor);
		environment_specularColor = CreateSharedPtrWithReleaseDLL(specularColor);
	}

	RenderTexture2D_Imp* Renderer3D::GetRenderTarget()
	{
		return m_renderTarget;
	}
}