#pragma once
#include <memory>
#include "../asd.ObjectManager.h"
#include "../Registration/asd.RegistrationCommand.h"
#include "asd.ICommitable.h"

namespace asd
{
	template<typename TOwner, typename TObject>
	class EventToManageObject : public ICommitable
	{
	private:
		std::shared_ptr<ObjectManager<TOwner, TObject>> m_manager;
		std::shared_ptr<TObject> m_object;
		RegistrationCommand m_command;
		bool m_raiseEvent;

	public:
		EventToManageObject(
			std::shared_ptr<ObjectManager<TOwner, TObject>> manager,
			std::shared_ptr<TObject> object,
			RegistrationCommand command,
			bool raiseEvent)
		{
		};

		void Commit()
		{
			switch (m_command)
			{
			case asd::RegistrationCommand::Add:
				m_manager->AddToContents(m_object, m_raiseEvent);
				break;

			case asd::RegistrationCommand::Remove:
				m_manager->RemoveFromContents(m_object, m_raiseEvent);
				break;
			}
		}
	};
}