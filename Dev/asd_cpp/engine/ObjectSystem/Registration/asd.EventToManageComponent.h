#pragma once
#include <memory>
#include "asd.ICommitable.h"
#include "asd.IImmediateComponentManager.h"
#include "asd.RegistrationCommand.h"

namespace asd
{
	template<typename TComponent>
	class EventToManageComponent : public ICommitable
	{
	private:
		typedef std::shared_ptr<TComponent> ComponentPtr;
		typedef std::shared_ptr<IImmediateComponentManager<TComponent>> ManagerPtr;

		ManagerPtr m_manager;
		ComponentPtr m_component;
		astring m_key;
		RegistrationCommand m_command;

	public:
		typedef std::shared_ptr<EventToManageComponent<TComponent>> Ptr;

		EventToManageComponent(
			ManagerPtr manager,
			ComponentPtr component,
			astring key,
			RegistrationCommand command)
			: m_manager(manager)
			, m_component(component)
			, m_key(key)
			, m_command(command)
		{
		}

		void Commit()
		{
			switch (m_command)
			{
			case asd::RegistrationCommand::Add:
				m_manager->ImmediatelyAddComponent(m_component, m_key);
				break;

			case asd::RegistrationCommand::Remove:
				m_manager->ImmediatelyRemoveComponent(m_key);
				break;
			}
		}

		static Ptr GetAddEvent(ManagerPtr manager, ComponentPtr component, astring key)
		{
			return std::make_shared<EventToManageComponent<TComponent>>(manager, component, key, RegistrationCommand::Add);
		}

		static Ptr GetRemoveEvent(ManagerPtr manager, astring key)
		{
			return std::make_shared<EventToManageComponent<TComponent>>(manager, nullptr, key, RegistrationCommand::Remove);
		}
	};
}