
#include "ace.CoreLayer3D_Imp.h"
#include "../PostEffect/ace.CorePostEffect_Imp.h"
#include "../../Window/ace.Window_Imp.h"
#include "../../Graphics/ace.Graphics_Imp.h"

#include "ace.CoreObject3D.h"
#include "ace.CoreObject3D_Imp.h"

#include "../../Core/ace.Core.h"

namespace ace
{
	CoreLayer3D_Imp::CoreLayer3D_Imp(Core* core, Graphics* graphics, Log* log, Vector2DI windowSize, RenderSettings settings)
		: CoreLayer_Imp(graphics, windowSize)
		, core(core)
		, m_objects(std::set<ObjectPtr>())
		, m_renderer(nullptr)
	{
		m_renderer = new Renderer3D(graphics, settings);
		m_renderer->SetWindowSize(windowSize);
	}

	CoreLayer3D_Imp::~CoreLayer3D_Imp()
	{
		for (auto& object : m_objects)
		{
			object->SetLayer(nullptr);
			object->Release();
		}
		m_objects.clear();

		SafeDelete(m_renderer);
	}

	RenderSettings CoreLayer3D_Imp::GetRenderSettings() const
	{
		return m_renderer->GetRenderSettings();
	}

	void CoreLayer3D_Imp::SetRenderSettings(RenderSettings settings)
	{
		m_renderer->SetRenderSettings(settings);
	}

	void CoreLayer3D_Imp::AddObject(ObjectPtr object3D)
	{
		if (m_objects.count(object3D) == 0)
		{
			m_objects.insert(object3D);
			object3D->SetLayer(this);
			SafeAddRef(object3D);
		}	
	}

	void CoreLayer3D_Imp::RemoveObject(ObjectPtr object3D)
	{
		if (m_objects.count(object3D) != 0)
		{
			m_objects.erase(object3D);
			object3D->SetLayer(nullptr);
			SafeRelease(object3D);
		}
	}

	float CoreLayer3D_Imp::GetAmbientColorIntensity()
	{
		return m_renderer->GetAmbientColorIntensity();
	}

	void CoreLayer3D_Imp::SetAmbientColorIntensity(float ambientColorIntensity)
	{
		m_renderer->SetAmbientColorIntensity(ambientColorIntensity);
	}

	void CoreLayer3D_Imp::SetSkyAmbientColor(Color color)
	{
		m_renderer->SetSkyAmbientColor(color);
	}

	void CoreLayer3D_Imp::SetGroundAmbientColor(Color color)
	{
		m_renderer->SetGroundAmbientColor(color);
	}

	void CoreLayer3D_Imp::SetEnvironmentColorIntensity(float environmentDiffuseColorIntensity, float environmentSpecularColorIntensity)
	{
		m_renderer->SetEnvironmentColorIntensity(environmentDiffuseColorIntensity, environmentSpecularColorIntensity);
	}

	void CoreLayer3D_Imp::SetEnvironmentColor(CubemapTexture* diffuseColor, CubemapTexture* specularColor)
	{
		m_renderer->SetEnvironmentColor(diffuseColor, specularColor);
	}

	float CoreLayer3D_Imp::GetSSAO_Radius()
	{
		return m_renderer->SSAO_Radius;
	}

	void CoreLayer3D_Imp::SetSSAO_Radius(float value)
	{
		m_renderer->SSAO_Radius = value;
	}

	float CoreLayer3D_Imp::GetSSAO_Bias()
	{
		return m_renderer->SSAO_Bias;
	}

	void CoreLayer3D_Imp::SetSSAO_Bias(float value)
	{
		m_renderer->SSAO_Bias = value;
	}

	float CoreLayer3D_Imp::GetSSAO_Intensity()
	{
		return m_renderer->SSAO_Intensity;
	}

	void CoreLayer3D_Imp::SetSSAO_Intensity(float value)
	{
		m_renderer->SSAO_Intensity = value;
	}

	float CoreLayer3D_Imp::GetSSAO_FarPlain()
	{
		return m_renderer->SSAO_FarPlain;
	}

	void CoreLayer3D_Imp::SetSSAO_FarPlain(float value)
	{
		m_renderer->SSAO_FarPlain = value;
	}

	void CoreLayer3D_Imp::DrawSpriteAdditionally(Vector3DF upperLeftPos, Vector3DF upperRightPos, Vector3DF lowerRightPos, Vector3DF lowerLeftPos,
		Color upperLeftCol, Color upperRightCol, Color lowerRightCol, Color lowerLeftCol,
		Vector2DF upperLeftUV, Vector2DF upperRightUV, Vector2DF lowerRightUV, Vector2DF lowerLeftUV,
		Texture2D* texture, AlphaBlend alphaBlend, bool depthWrite, bool depthTest)
	{
		m_renderer->DrawSpriteAdditionally(
			upperLeftPos, upperRightPos, lowerRightPos, lowerLeftPos,
			upperLeftCol, upperRightCol, lowerRightCol, lowerLeftCol,
			upperLeftUV, upperRightUV, lowerRightUV, lowerLeftUV,
			texture, alphaBlend, depthWrite, depthTest);
	}

	void CoreLayer3D_Imp::BeginUpdating(bool isUpdated)
	{
		auto deltaTime = core->GetDeltaTime();
		if (!isUpdated)
		{
			deltaTime = 0;
		}

		m_renderer->Flip(deltaTime);

		m_isDrawnTemp = m_isDrawn;

		if (m_isDrawnTemp)
		{
			m_renderer->BeginRendering(deltaTime);
		}
	}

	void CoreLayer3D_Imp::EndUpdating(bool isUpdated)
	{
		if (m_isDrawnTemp)
		{
			m_renderer->EndRendering();
		}
	}

	void CoreLayer3D_Imp::BeginDrawing()
	{
	}

	void CoreLayer3D_Imp::Draw()
	{
		if (!m_isDrawn)
		{
			return;
		}
	}

	void CoreLayer3D_Imp::EndDrawing()
	{
		m_scene->SetRenderTargetForDrawingLayer();

		m_renderer->RenderResult();
	}
};