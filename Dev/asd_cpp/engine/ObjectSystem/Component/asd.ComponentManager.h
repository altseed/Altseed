#pragma once
#include <memory>
#include <map>
#include <list>
#include "../../asd.CoreToEngine.h"
#include "../Registration/asd.IComponentRegisterable.h"
#include "../Registration/asd.IImmediateComponentManager.h"

namespace asd
{
	template<typename TComponent>
	class ComponentManager : IImmediateComponentManager<TComponent>
	{
	private:
		typedef std::shared_ptr<TComponent> ComponentPtr;

		IComponentRegisterable<TComponent>* m_owner;
		std::map<astring, ComponentPtr> m_components;

	public:
		ComponentManager(IComponentRegisterable<TComponent>* owner)
			: m_owner(owner)
			, m_components(std::map<astring, ComponentPtr>())
			, m_beAdded(std::map<astring, ComponentPtr>())
			, m_beRemoved(std::list<astring>())
			, m_isUpdating(false)
		{
		}

		void Add(const ComponentPtr& component, astring key)
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

		ComponentPtr Get(astring key)
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

		bool Remove(astring key)
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

		void Update()
		{
			for (auto& c : m_components)
			{
				c.second->Update();
			}
		}
	};
}