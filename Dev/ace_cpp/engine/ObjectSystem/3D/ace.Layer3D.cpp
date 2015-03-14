
#include "ace.Layer3D.h"

namespace ace
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

	void Layer3D::Update()
	{
		if (!m_isUpdated || !m_isAlive)
		{
			return;
		}

		OnUpdating();

		m_objects.Update();

		/*
		for (auto& component : m_components)
		{
			component.second->Update();
		}
		*/

		OnUpdated();
	}

	void Layer3D::CallDestroy()
	{
		for (auto& o : m_objects.GetContents())
		{
			if (o->GetIsAlive())
			{
				o->CallDestroy();
			}
		}
		OnDispose();
	}

	void Layer3D::BeginUpdating()
	{
		m_coreLayer->BeginUpdating();
	}

	void Layer3D::EndUpdateting()
	{
		m_coreLayer->EndUpdating();
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

	void Layer3D::OnUpdating()
	{
	}

	void Layer3D::OnUpdated()
	{
	}

	void Layer3D::OnDrawAdditionally()
	{
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
		if (object->GetLayer() != nullptr)
		{
			throw "追加しようとしたオブジェクトは、すでに別のレイヤーに所属しています。";
		}
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
		std::shared_ptr<Texture2D>  texture, AlphaBlend alphaBlend, bool depthWrite, bool depthTest)
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

	bool Layer3D::GetHDRMode() const
	{
		return m_coreLayer->GetHDRMode();
	}

	void Layer3D::SetHDRMode(bool value)
	{
		m_coreLayer->SetHDRMode(value);
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
};