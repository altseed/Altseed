
#include "asd.Layer3D.h"
#include <Utility/asd.Timer.h>

namespace asd
{

	extern ObjectSystemFactory* g_objectSystemFactory;

	Layer3D::Layer3D(RenderSettings settings)
		: m_coreLayer(nullptr)
		, m_objects()
	{
		m_coreLayer = CreateSharedPtrWithReleaseDLL(g_objectSystemFactory->CreateLayer3D(settings));
		m_commonObject = m_coreLayer;
	}

	Layer3D::~Layer3D()
	{
		for (auto& object : m_objects.GetContents())
		{
			object->SetLayer(nullptr);
		}
	}

	void Layer3D::UpdateInternal()
	{
		m_objects.Update();

		for (auto& vanishing : m_objects.GetVanishingContents())
		{
			RemoveObject(vanishing);
			vanishing->Dispose();
		}
		m_objects.GetVanishingContents().clear();
	}

	void Layer3D::Dispose()
	{
		for (auto& o : m_objects.GetContents())
		{
			if (o->GetIsAlive())
			{
				o->Dispose();
			}
		}
		OnDispose();
	}

	void Layer3D::BeginUpdating()
	{
		m_isUpdatedCurrent = m_isUpdated;
		m_coreLayer->BeginUpdating(m_isUpdatedCurrent);
	}

	void Layer3D::EndUpdateting()
	{
		m_coreLayer->EndUpdating(m_isUpdatedCurrent);
	}

	void Layer3D::DrawAdditionally()
	{
		if (!m_isDrawn)
		{
			return;
		}

		for (auto& object : m_objects.GetContents())
		{
			object->OnDrawAdditionally();
		}

		OnDrawAdditionally();
	}

	RenderSettings Layer3D::GetRenderSettings() const
	{
		return m_coreLayer->GetRenderSettings();
	}

	void Layer3D::SetRenderSettings(RenderSettings settings)
	{
		m_coreLayer->SetRenderSettings(settings);
	}

	void Layer3D::AddObject(const Object3D::Ptr& object)
	{
		ACE_ASSERT(object->GetLayer() == nullptr, "追加しようとしたオブジェクトは、すでに別のレイヤーに所属しています。");
		m_objects.Add(object);
		auto coreObj = object->GetCoreObject();
		m_coreLayer->AddObject(coreObj);
		object->SetLayer(this);
		object->Start();
	}

	void Layer3D::RemoveObject(const Object3D::Ptr& object)
	{
		m_objects.Remove(object);
		m_coreLayer->RemoveObject(object->GetCoreObject());
		object->SetLayer(nullptr);
	}

	const std::list<Object3D::Ptr>& Layer3D::GetObjects() const
	{
		return m_objects.GetContents();
	}

	void Layer3D::DrawSpriteAdditionally(Vector3DF upperLeftPos, Vector3DF upperRightPos, Vector3DF lowerRightPos, Vector3DF lowerLeftPos,
		Color upperLeftCol, Color upperRightCol, Color lowerRightCol, Color lowerLeftCol,
		Vector2DF upperLeftUV, Vector2DF upperRightUV, Vector2DF lowerRightUV, Vector2DF lowerLeftUV,
		std::shared_ptr<Texture2D>  texture, AlphaBlendMode alphaBlend, bool depthWrite, bool depthTest)
	{
		m_coreLayer->DrawSpriteAdditionally(
			upperLeftPos, upperRightPos, lowerRightPos, lowerLeftPos,
			upperLeftCol, upperRightCol, lowerRightCol, lowerLeftCol,
			upperLeftUV, upperRightUV, lowerRightUV, lowerLeftUV,
			texture.get(), alphaBlend, depthWrite, depthTest);
	}

	float Layer3D::GetAmbientColorIntensity()
	{
		return m_coreLayer->GetAmbientColorIntensity();
	}

	void Layer3D::SetAmbientColorIntensity(float ambientColorIntensity)
	{
		m_coreLayer->SetAmbientColorIntensity(ambientColorIntensity);
	}

	void Layer3D::SetSkyAmbientColor(Color color)
	{
		m_coreLayer->SetSkyAmbientColor(color);
	}

	void Layer3D::SetGroundAmbientColor(Color color)
	{
		m_coreLayer->SetGroundAmbientColor(color);
	}

	void Layer3D::SetEnvironmentColorIntensity(float environmentDiffuseColorIntensity, float environmentSpecularColorIntensity)
	{
		m_coreLayer->SetEnvironmentColorIntensity(
			environmentDiffuseColorIntensity,
			environmentSpecularColorIntensity);
	}

	void Layer3D::SetEnvironmentColor(std::shared_ptr<CubemapTexture> diffuseColor, std::shared_ptr<CubemapTexture> specularColor)
	{
		m_coreLayer->SetEnvironmentColor(diffuseColor.get(), specularColor.get());
	}

	float Layer3D::GetSSAO_Radius()
	{
		return m_coreLayer->GetSSAO_Radius();
	}

	void Layer3D::SetSSAO_Radius(float value)
	{
		m_coreLayer->SetSSAO_Radius(value);
	}

	float Layer3D::GetSSAO_Bias()
	{
		return m_coreLayer->GetSSAO_Bias();
	}

	void Layer3D::SetSSAO_Bias(float value)
	{
		m_coreLayer->SetSSAO_Bias(value);
	}

	float Layer3D::GetSSAO_Intensity()
	{
		return m_coreLayer->GetSSAO_Intensity();
	}

	void Layer3D::SetSSAO_Intensity(float value)
	{
		m_coreLayer->SetSSAO_Intensity(value);
	}

	float Layer3D::GetSSAO_FarPlain()
	{
		return m_coreLayer->GetSSAO_FarPlain();
	}

	void Layer3D::SetSSAO_FarPlain(float value)
	{
		m_coreLayer->SetSSAO_FarPlain(value);
	}

	bool Layer3D::GetIsHeightFogEnabled()
	{
		return m_coreLayer->GetIsHeightFogEnabled();
	}

	void Layer3D::SetIsHeightFogEnabled(bool value)
	{
		m_coreLayer->SetIsHeightFogEnabled(value);
	}

	float Layer3D::GetHeightFogDensity()
	{
		return m_coreLayer->GetHeightFogDensity();
	}

	void Layer3D::SetHeightFogDensity(float value)
	{
		m_coreLayer->SetHeightFogDensity(value);
	}

	Color Layer3D::GetHeightFogColor()
	{
		return m_coreLayer->GetHeightFogColor();
	}

	void Layer3D::SetHeightFogColor(Color value)
	{
		m_coreLayer->SetHeightFogColor(value);
	}

	float Layer3D::GetHeightFogFalloff()
	{
		return m_coreLayer->GetHeightFogFalloff();
	}

	void Layer3D::SetHeightFogFalloff(float value)
	{
		m_coreLayer->SetHeightFogFalloff(value);
	}

	float Layer3D::GetHeightFogStartDistance()
	{
		return m_coreLayer->GetHeightFogFalloff();
	}

	void Layer3D::SetHeightFogStartDistance(float value)
	{
		m_coreLayer->SetHeightFogStartDistance(value);
	}

	bool Layer3D::GetIsOceanEnabled()
	{
		return m_coreLayer->GetIsOceanEnabled();
	}

	void Layer3D::SetIsOceanEnabled(bool value)
	{
		m_coreLayer->SetIsOceanEnabled(value);
	}

	RectF Layer3D::GetOceanArea()
	{
		return m_coreLayer->GetOceanArea();
	}

	void Layer3D::SetOceanArea(RectF value)
	{
		m_coreLayer->SetOceanArea(value);
	}

	float Layer3D::GetOceanHeight()
	{
		return m_coreLayer->GetOceanHeight();
	}

	void Layer3D::SetOceanHeight(float value)
	{
		m_coreLayer->SetOceanHeight(value);
	}

	float Layer3D::GetOceanGridSize()
	{
		return m_coreLayer->GetOceanGridSize();
	}

	void Layer3D::SetOceanGridSize(float value)
	{
		m_coreLayer->SetOceanGridSize(value);
	}

	Texture2D* Layer3D::GetOceanNormalMap()
	{
		return m_coreLayer->GetOceanNormalMap();
	}

	void Layer3D::SetOceanNormalMap(std::shared_ptr<Texture2D> value)
	{
		m_coreLayer->SetOceanNormalMap(value.get());
	}

	int Layer3D::GetObjectCount() const
	{
		return m_objects.GetContents().size();
	}
};