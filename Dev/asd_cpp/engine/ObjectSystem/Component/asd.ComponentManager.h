#pragma once
#include <memory>
#include <map>
#include <list>
#include "../../asd.CoreToEngine.h"

namespace asd
{
	template<typename TOwner, typename TComponent>
	class ComponentManager
	{
	private:
		typedef std::shared_ptr<TComponent> ComponentPtr;
		TOwner* m_owner;
		std::map<astring, ComponentPtr> m_components;
		std::map<astring, ComponentPtr> m_beAdded;
		std::list<astring> m_beRemoved;
		bool m_isUpdating;

	public:
		ComponentManager(TOwner* owner);

		void Add(const ComponentPtr& component, astring key);

		ComponentPtr Get(astring key);

		bool Remove(astring key);

		void Update();
	};

	template<typename TOwner, typename TComponent>
	ComponentManager<TOwner, TComponent>::ComponentManager(TOwner* owner)
		: m_owner(owner)
		, m_components(std::map<astring, ComponentPtr>())
		, m_beAdded(std::map<astring, ComponentPtr>())
		, m_beRemoved(std::list<astring>())
		, m_isUpdating(false)
	{
	}

	template<typename TOwner, typename TComponent>
	void ComponentManager<TOwner, TComponent>::Add(const ComponentPtr& component, astring key)
	{
		component->SetOwner(m_owner);
		if (m_isUpdating)
		{
			m_beAdded[key] = component;
		}
		else
		{
			m_components[key] = component;
		}
	}

	template<typename TOwner, typename TComponent>
	std::shared_ptr<TComponent> ComponentManager<TOwner, TComponent>::Get(astring key)
	{
		if (std::find(m_beRemoved.begin(), m_beRemoved.end(), key) != m_beRemoved.end())
		{
			return nullptr;
		}

		auto it = m_components.find(key);
		if (it != m_components.end())
		{
			return it->second;
		}

		it = m_beAdded.find(key);
		if (it != m_beAdded.end())
		{
			return it->second;
		}

		return nullptr;
	}

	template<typename TOwner, typename TComponent>
	bool ComponentManager<TOwner, TComponent>::Remove(astring key)
	{
		auto c = Get(key);
		if (c != nullptr)
		{
			c->SetOwner(nullptr);
			if (m_isUpdating)
			{
				m_beRemoved.push_back(key);
			}
			else
			{
				m_components.erase(key);
			}
			return true;
		}
		else
		{
			return false;
		}
	}

	template<typename TOwner, typename TComponent>
	void ComponentManager<TOwner, TComponent>::Update()
	{
		m_isUpdating = true;
		auto beVanished = std::list<astring>();
		for (auto& c : m_components)
		{
			c.second->Update();
			if (!c.second->GetIsAlive())
			{
				beVanished.push_back(c.first);
			}
		}
		m_isUpdating = false;

		for (auto& key : beVanished)
		{
			m_components.erase(key);
		}

		for (auto& c : m_beAdded)
		{
			m_components.insert(c);
		}

		for (auto& key : m_beRemoved)
		{
			m_components.erase(key);
		}

		m_beAdded.clear();
		m_beRemoved.clear();
	}
}