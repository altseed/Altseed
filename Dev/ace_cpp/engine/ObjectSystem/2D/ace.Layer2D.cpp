#include <list>
#include <exception>
#include "ace.Layer2D.h"
using namespace std;

namespace ace
{
	extern ObjectSystemFactory* g_objectSystemFactory;

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Layer2D::Layer2D()
		: m_coreLayer(nullptr)
		, m_objects(list<ObjectPtr>())
		, m_components(map<astring, ComponentPtr>())
	{
		m_coreLayer = CreateSharedPtrWithReleaseDLL(g_objectSystemFactory->CreateLayer2D());
		m_commonObject = m_coreLayer;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Layer2D::~Layer2D()
	{
		for (auto& object : m_objects)
		{
			object->SetLayer(nullptr);
		}
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Layer2D::Update()
	{
		if (!m_isUpdated)
		{
			return;
		}

		OnUpdating();

		auto beVanished = vector<ObjectPtr>();

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

		for (auto& component : m_components)
		{
			component.second->Update();
		}

		OnUpdated();
	}

	void Layer2D::BeginUpdateting()
	{
		m_coreLayer->BeginUpdating();
	}

	void Layer2D::EndUpdateting()
	{
		m_coreLayer->EndUpdating();
	}

	void Layer2D::DrawAdditionally()
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

	void Layer2D::OnUpdating()
	{
	}

	void Layer2D::OnUpdated()
	{
	}

	void Layer2D::OnDrawAdditionally()
	{
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Layer2D::AddObject(const ObjectPtr& object)
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

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Layer2D::RemoveObject(const ObjectPtr& object)
	{
		m_objects.remove(object);
		m_coreLayer->RemoveObject(object->GetCoreObject());
		object->SetLayer(nullptr);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Layer2D::AddComponent(const ComponentPtr& component, astring key)
	{
		m_components[key] = component;
		component->SetOwner(this);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Layer2D::ComponentPtr& Layer2D::GetComponent(astring key)
	{
		return m_components[key];
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Layer2D::RemoveComponent(astring key)
	{
		auto it = m_components.find(key);
		m_components.erase(it);
		it->second->SetOwner(nullptr);
	}

}
