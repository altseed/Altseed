
#include <list>
#include "ace.CoreLayer2D_Imp.h"
#include "../PostEffect/ace.CorePostEffect_Imp.h"
#include "../../Window/ace.Window_Imp.h"
#include "../../Graphics/Common/ace.Graphics_Imp.h"

using namespace std;

namespace ace
{
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	CoreLayer2D_Imp::CoreLayer2D_Imp(Graphics* graphics, Log* log, Vector2DI windowSize)
		: m_objects(list<ObjectPtr>())
		, m_graphics(nullptr)
		, m_renderer(nullptr)
		, m_layerRenderer(nullptr)
		, m_drawingPriority(0)
		, m_renderTargetDefaultToPostEffect(nullptr)
		, m_renderTargetPostEffectToLayer(nullptr)
		, m_layerSize(windowSize)
	{
		m_renderer = new Renderer2D_Imp(graphics, log, windowSize);
		m_layerRenderer = new LayerRenderer(graphics);
		m_layerRenderer->SetWindowSize(windowSize);

		{
			ace::Vector2DF lpos[4];
			lpos[0].X = 0;
			lpos[0].Y = 0;
			lpos[1].X = windowSize.X;
			lpos[1].Y = 0;
			lpos[2].X = windowSize.X;
			lpos[2].Y = windowSize.Y;
			lpos[3].X = 0;
			lpos[3].Y = windowSize.Y;
			m_layerRenderer->SetLayerPosition(lpos);
		}

		m_graphics = (Graphics_Imp*) graphics;
		SafeAddRef(m_graphics);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	CoreLayer2D_Imp::~CoreLayer2D_Imp()
	{
		ClearPostEffects();

		SafeRelease(m_renderTargetDefaultToPostEffect);
		SafeRelease(m_renderTargetPostEffectToLayer);

		SafeDelete(m_renderer);
		SafeRelease(m_layerRenderer);

		for (auto& object : m_objects)
		{
			SafeRelease(object);
		}

		SafeRelease(m_graphics);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreLayer2D_Imp::AddObject(ObjectPtr object)
	{
		m_objects.push_back(object);
		object->SetLayer(this);
		SafeAddRef(object);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreLayer2D_Imp::RemoveObject(ObjectPtr object)
	{
		m_objects.remove(object);
		object->SetLayer(nullptr);
		SafeRelease(object);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreLayer2D_Imp::BeginDrawing()
	{
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreLayer2D_Imp::Draw()
	{
		for (auto& x : m_objects)
		{
			x->Draw();
		}
		
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreLayer2D_Imp::EndDrawing()
	{
		if (m_postEffects.size() > 0)
		{
			m_graphics->SetRenderTarget(m_renderTargetDefaultToPostEffect, nullptr);
		}

		m_renderer->DrawCache();
		m_renderer->ClearCache();
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreLayer2D_Imp::EndDrawingAfterEffects()
	{
		if (m_postEffects.size() > 0)
		{
			m_graphics->SetRenderTarget(nullptr, nullptr);
			m_layerRenderer->SetTexture(m_renderTargetPostEffectToLayer);

			{
				ace::Vector2DF positions[4];
				ace::Color colors[4];
				ace::Vector2DF uvs[4];

				colors[0] = ace::Color(255, 255, 255, 255);
				colors[1] = ace::Color(255, 255, 255, 255);
				colors[2] = ace::Color(255, 255, 255, 255);

				positions[0].X = -1.0f;
				positions[0].Y = -1.0f;
				positions[1].X = 1.0f;
				positions[1].Y = -1.0f;
				positions[2].X = 1.0f;
				positions[2].Y = 1.0f;


				uvs[0].X = 0;
				uvs[0].Y = 0;
				uvs[1].X = 1;
				uvs[1].Y = 0;
				uvs[2].X = 1;
				uvs[2].Y = 1;

				m_layerRenderer->AddTriangle(positions, colors, uvs);
			}

			{
				ace::Vector2DF positions[4];
				ace::Color colors[4];
				ace::Vector2DF uvs[4];

				colors[0] = ace::Color(255, 255, 255, 255);
				colors[1] = ace::Color(255, 255, 255, 255);
				colors[2] = ace::Color(255, 255, 255, 255);

				positions[0].X = -1.0f;
				positions[0].Y = 1.0f;
				positions[1].X = 1.0f;
				positions[1].Y = 1.0f;
				positions[2].X = -1.0f;
				positions[2].Y = -1.0f;

				uvs[0].X = 0;
				uvs[0].Y = 1;
				uvs[1].X = 1;
				uvs[1].Y = 1;
				uvs[2].X = 0;
				uvs[2].Y = 0;

				m_layerRenderer->AddTriangle(positions, colors, uvs);
			}

			m_layerRenderer->DrawCache();
			m_layerRenderer->ClearCache();
		}
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreLayer2D_Imp::AddPostEffect(CorePostEffect* postEffect)
	{
		SafeAddRef(postEffect);
		m_postEffects.push_back(postEffect);

		if (m_renderTargetDefaultToPostEffect == nullptr)
		{
			m_renderTargetDefaultToPostEffect = m_graphics->CreateRenderTexture_Imp(m_layerSize.X, m_layerSize.Y, eTextureFormat::TEXTURE_FORMAT_RGBA8888);
		}

		if (m_renderTargetPostEffectToLayer == nullptr)
		{
			m_renderTargetPostEffectToLayer = m_graphics->CreateRenderTexture_Imp(m_layerSize.X, m_layerSize.Y, eTextureFormat::TEXTURE_FORMAT_RGBA8888);
		}
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreLayer2D_Imp::ClearPostEffects()
	{
		for (auto& p : m_postEffects)
		{
			SafeRelease(p);
		}

		m_postEffects.clear();
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	RenderTexture2D* CoreLayer2D_Imp::GetRenderTargetDefaultToPostEffect()
	{
		return m_renderTargetDefaultToPostEffect;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	RenderTexture2D* CoreLayer2D_Imp::GetRenderTargetPostEffectToLayer()
	{
		return m_renderTargetPostEffectToLayer;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Renderer2D* CoreLayer2D_Imp::GetRenderer() const
	{
		return m_renderer;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	int CoreLayer2D_Imp::GetDrawingPriority() const
	{
		return m_drawingPriority;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreLayer2D_Imp::SetDrawingPriority(int value)
	{
		m_drawingPriority = value;
	}

}