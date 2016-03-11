
#include "asd.CoreLayer3D_Imp.h"
#include "../PostEffect/asd.CorePostEffect_Imp.h"
#include "../../Window/asd.Window_Imp.h"
#include "../../Graphics/asd.Graphics_Imp.h"

#include "asd.CoreObject3D.h"
#include "asd.CoreObject3D_Imp.h"

#include "../../Core/asd.Core.h"

namespace asd
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

	void CoreLayer3D_Imp::SetSize(Vector2DI size)
	{

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

	bool CoreLayer3D_Imp::GetIsHeightFogEnabled()
	{
		return m_renderer->IsHeightFogEnabled;
	}

	void CoreLayer3D_Imp::SetIsHeightFogEnabled(bool value)
	{
		m_renderer->IsHeightFogEnabled = value;
	}

	float CoreLayer3D_Imp::GetHeightFogDensity()
	{
		return m_renderer->HeightFogDensity;
	}

	void CoreLayer3D_Imp::SetHeightFogDensity(float value)
	{
		m_renderer->HeightFogDensity = value;
	}

	Color CoreLayer3D_Imp::GetHeightFogColor()
	{
		return m_renderer->HeightFogColor;
	}

	void CoreLayer3D_Imp::SetHeightFogColor(Color value)
	{
		m_renderer->HeightFogColor = value;
	}

	float CoreLayer3D_Imp::GetHeightFogFalloff()
	{
		return m_renderer->HeightFogFalloff;
	}

	void CoreLayer3D_Imp::SetHeightFogFalloff(float value)
	{
		m_renderer->HeightFogFalloff = value;
	}

	float CoreLayer3D_Imp::GetHeightFogStartDistance()
	{
		return m_renderer->HeightFogStartDistance;
	}

	void CoreLayer3D_Imp::SetHeightFogStartDistance(float value)
	{
		m_renderer->HeightFogStartDistance = value;
	}

	bool CoreLayer3D_Imp::GetIsOceanEnabled()
	{
		return m_renderer->IsOceanEnabled;
	}

	void CoreLayer3D_Imp::SetIsOceanEnabled(bool value)
	{
		m_renderer->IsOceanEnabled = value;
	}

	RectF CoreLayer3D_Imp::GetOceanArea()
	{
		return m_renderer->OceanArea;
	}

	void CoreLayer3D_Imp::SetOceanArea(RectF value)
	{
		m_renderer->OceanArea = value;
	}

	float CoreLayer3D_Imp::GetOceanHeight()
	{
		return m_renderer->OceanHeight;
	}

	void CoreLayer3D_Imp::SetOceanHeight(float value)
	{
		m_renderer->OceanHeight = value;
	}

	float CoreLayer3D_Imp::GetOceanGridSize()
	{
		return m_renderer->OceanGridSize;
	}

	void CoreLayer3D_Imp::SetOceanGridSize(float value)
	{
		m_renderer->OceanGridSize = value;
	}

	Color CoreLayer3D_Imp::GetOceanColor()
	{
		return m_renderer->OceanColor;
	}

	void CoreLayer3D_Imp::SetOceanColor(Color color)
	{
		m_renderer->OceanColor = color;
	}

	float CoreLayer3D_Imp::GetOceanDensity()
	{
		return m_renderer->OceanDensity;
	}

	void CoreLayer3D_Imp::SetOceanDensity(float value)
	{
		m_renderer->OceanDensity = value;
	}

	Texture2D* CoreLayer3D_Imp::GetOceanNormalMap()
	{
		return m_renderer->OceanNormalMap.get();  
	}

	void CoreLayer3D_Imp::SetOceanNormalMap(Texture2D* value)
	{
		SafeAddRef(value);
		m_renderer->OceanNormalMap = CreateSharedPtrWithReleaseDLL(value);
	}

	Texture2D* CoreLayer3D_Imp::GetOceanMask()
	{
		return m_renderer->OceanMask.get();
	}
	void CoreLayer3D_Imp::SetOceanMask(Texture2D* value)
	{
		SafeAddRef(value);
		m_renderer->OceanMask = CreateSharedPtrWithReleaseDLL(value);
	}

	void CoreLayer3D_Imp::DrawSpriteAdditionally(Vector3DF upperLeftPos, Vector3DF upperRightPos, Vector3DF lowerRightPos, Vector3DF lowerLeftPos,
		Color upperLeftCol, Color upperRightCol, Color lowerRightCol, Color lowerLeftCol,
		Vector2DF upperLeftUV, Vector2DF upperRightUV, Vector2DF lowerRightUV, Vector2DF lowerLeftUV,
		Texture2D* texture, AlphaBlendMode alphaBlend, bool depthWrite, bool depthTest)
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