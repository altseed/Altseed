
#include "ace.Layer3D.h"

namespace ace
{

	extern ObjectSystemFactory* g_objectSystemFactory;

	Layer3D::Layer3D(RenderSettings settings)
		: m_coreLayer(nullptr)
	{
		m_coreLayer = CreateSharedPtrWithReleaseDLL(g_objectSystemFactory->CreateLayer3D(settings));
		m_commonObject = m_coreLayer;
	}

	Layer3D::~Layer3D()
	{
		for (auto& object : m_objects)
		{
			object->SetLayer(nullptr);
		}
	}

	void Layer3D::Update()
	{
		if (!m_isUpdated)
		{
			return;
		}

		OnUpdating();

		
		auto beVanished = std::vector<ObjectPtr>();

		for (auto& object : m_objects)
		{
			object->Update();
			if (!object->GetIsAlive())
			{
				beVanished.push_back(object);
			}
		}

		for (auto& object : beVanished)
		{
			RemoveObject(object);
		}

		/*
		for (auto& component : m_components)
		{
			component.second->Update();
		}
		*/

		OnUpdated();
	}

	void Layer3D::BeginUpdateting()
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

		for (auto& object : m_objects)
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

	void Layer3D::AddObject(const ObjectPtr& object)
	{
		if (object->GetLayer() != nullptr)
		{
			throw "追加しようとしたオブジェクトは、すでに別のレイヤーに所属しています。";
		}
		m_objects.push_back(object);
		auto coreObj = object->GetCoreObject();
		m_coreLayer->AddObject(coreObj);
		object->SetLayer(this);
		object->Start();
	}

	void Layer3D::RemoveObject(const ObjectPtr& object)
	{
		m_objects.remove(object);
		m_coreLayer->RemoveObject(object->GetCoreObject());
		object->SetLayer(nullptr);
	}

	void Layer3D::SetSkyAmbientColor(Color color)
	{
		m_coreLayer->SetSkyAmbientColor(color);
	}

	void Layer3D::SetGroundAmbientColor(Color color)
	{
		m_coreLayer->SetGroundAmbientColor(color);
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