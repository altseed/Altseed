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
		, m_objects(list<Object2D::Ptr>())
		, m_components(map<astring, Layer2DComponent::Ptr>())
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

	void Layer2D::UpdateComponents()
	{
		auto beVanished = vector<astring>();
		for (auto& component : m_components)
		{
			component.second->Update();
			if (!component.second->GetIsAlive())
			{
				beVanished.push_back(component.first);
			}
		}

		for (auto& x : beVanished)
		{
			RemoveComponent(x);
		}
	}

	void Layer2D::UpdateObjects()
	{
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

		beVanished.clear();
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
		UpdateObjects();
		UpdateComponents();
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

	void Layer2D::DrawSpriteAdditionally(Vector2DF upperLeftPos, Vector2DF upperRightPos, Vector2DF lowerRightPos, Vector2DF lowerLeftPos,
		Color upperLeftCol, Color upperRightCol, Color lowerRightCol, Color lowerLeftCol,
		Vector2DF upperLeftUV, Vector2DF upperRightUV, Vector2DF lowerRightUV, Vector2DF lowerLeftUV,
		std::shared_ptr<Texture2D>  texture, AlphaBlend alphaBlend, int32_t priority)
	{
		m_coreLayer->DrawSpriteAdditionally(upperLeftPos, upperRightPos, lowerRightPos, lowerLeftPos, upperLeftCol, upperRightCol, lowerRightCol, lowerLeftCol, upperLeftUV, upperRightUV, lowerRightUV, lowerLeftUV, texture.get(), alphaBlend, priority);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Layer2D::AddObject(const Object2D::Ptr& object)
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
	void Layer2D::RemoveObject(const Object2D::Ptr& object)
	{
		m_objects.remove(object);
		m_coreLayer->RemoveObject(object->GetCoreObject());
		object->SetLayer(nullptr);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Layer2D::AddComponent(const Layer2DComponent::Ptr& component, astring key)
	{
		m_components[key] = component;
		component->SetOwner(this);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Layer2DComponent::Ptr& Layer2D::GetComponent(astring key)
	{
		return m_components[key];
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Layer2D::RemoveComponent(astring key)
	{
		auto it = m_components.find(key);
		it->second->SetOwner(nullptr);
		m_components.erase(it);
	}

}
