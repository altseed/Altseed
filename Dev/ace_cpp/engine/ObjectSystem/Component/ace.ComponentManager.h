#pragma once
#include <map>
#include <list>
#include "../../ace.CoreToEngine.h"

namespace ace
{
	template<typename TOwner, typename TComponentPtr>
	class ComponentManager
	{
	private:
		TOwner* m_owner;
		std::map<astring, TComponentPtr> m_components;
		std::map<astring, TComponentPtr> m_componentsToBeAdded;

	public:
		ComponentManager(TOwner* owner);

		void Add(const TComponentPtr& component, astring key);

		TComponentPtr Get(astring key);

		bool Remove(astring key);

		void Update();
	};

	template<typename TOwner, typename TComponentPtr>
	ComponentManager<TOwner, TComponentPtr>::ComponentManager(TOwner* owner)
		: m_owner(owner)
		, m_components(std::map<astring, Object2DComponent::Ptr>())
		, m_componentsToBeAdded(std::map<astring, Object2DComponent::Ptr>())
	{
	}

	template<typename TOwner, typename TComponentPtr>
	void ComponentManager<TOwner, TComponentPtr>::Add(const TComponentPtr& component, astring key)
	{
		m_componentsToBeAdded[key] = component;
		component->SetOwner(m_owner);
	}

	template<typename TOwner, typename TComponentPtr>
	TComponentPtr ComponentManager<TOwner, TComponentPtr>::Get(astring key)
	{
		if (m_components.find(key) != m_components.end())
		{
			return m_components[key];
		}
		else
		{
			return m_componentsToBeAdded[key];
		}
	}

	template<typename TOwner, typename TComponentPtr>
	bool ComponentManager<TOwner, TComponentPtr>::Remove(astring key)
	{
		if (m_components.find(key) != m_components.end())
		{
			m_components[key]->SetOwner(nullptr);
			m_components.erase(key);
			return true;
		}
		else if (m_componentsToBeAdded.find(key) != m_componentsToBeAdded.end())
		{
			m_componentsToBeAdded[key]->SetOwner(nullptr);
			m_componentsToBeAdded.erase(key);
			return true;
		}
		else
		{
			return false;
		}
	}

	template<typename TOwner, typename TComponentPtr>
	void ComponentManager<TOwner, TComponentPtr>::Update()
	{
		for (auto& c : m_componentsToBeAdded)
		{
			m_components.insert(c);
		}
		m_componentsToBeAdded.clear();

		auto beVanished = std::list<astring>();
		for (auto& c : m_components)
		{
			c.second->Update();
			if (!c.second->GetIsAlive())
			{
				beVanished.push_back(c.first);
			}
		}

		for (auto& key : beVanished)
		{
			m_components.erase(key);
		}
	}
}