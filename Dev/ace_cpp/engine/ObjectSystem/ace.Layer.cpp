#include "ace.Layer.h"
using namespace std;

namespace ace
{
	Layer::Layer()
		: m_components(map<astring, ComponentPtr>())
		, m_scene(nullptr)
	{
	}

	void Layer::Update()
	{
		OnUpdated();
		UpdateObjects();

		for (auto& component : m_components)
		{
			component.second->Update();
		}

		OnUpdating();
	}

	void Layer::AddComponent(const ComponentPtr& component, astring key)
	{
		m_components[key] = component;
		component->SetLayer(this);
	}

	Layer::ComponentPtr& Layer::GetComponent(astring key)
	{
		return m_components[key];
	}

	void Layer::RemoveComponent(astring key)
	{
		auto it = m_components.find(key);
		m_components.erase(it);
		it->second->SetLayer(nullptr);
	}

	Scene* Layer::GetScene() const
	{
		return m_scene;
	}

	void Layer::SetScene(Scene* value)
	{
		m_scene = value;
	}
}