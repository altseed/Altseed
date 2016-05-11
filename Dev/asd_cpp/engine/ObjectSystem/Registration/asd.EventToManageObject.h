#pragma once
#include <memory>
#include "../Registration/asd.RegistrationCommand.h"
#include "asd.ICommitable.h"
#include "asd.IImmediateObjectManager.h"

namespace asd
{
	template<typename TObject>
	class EventToManageObject : public ICommitable
	{
	private:
		std::shared_ptr<ImmediateObjectManager<TObject>> m_manager;
		std::shared_ptr<TObject> m_object;
		RegistrationCommand m_command;
		bool m_raiseEvent;

	public:
		EventToManageObject(
			std::shared_ptr<ImmediateObjectManager<TObject>> manager,
			std::shared_ptr<TObject> object,
			RegistrationCommand command,
			bool raiseEvent)
			: m_manager(manager)
			, m_object(object)
			, m_command(command)
			, m_raiseEvent(raiseEvent)
		{
		};

		void Commit()
		{
			switch (m_command)
			{
			case asd::RegistrationCommand::Add:
				m_manager->ImmediatelyAddObject(m_object, m_raiseEvent);
				break;

			case asd::RegistrationCommand::Remove:
				m_manager->ImmediatelyRemoveObject(m_object, m_raiseEvent);
				break;
			}
		}
	};
}