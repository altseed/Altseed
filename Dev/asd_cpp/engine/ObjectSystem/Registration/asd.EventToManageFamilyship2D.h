#pragma once
#include "asd.ICommitable.h"
#include "asd.RegistrationCommand.h"
#include "../2D/asd.Object2D.h"

namespace asd
{
	class EventToManageFamilyship2D : public ICommitable
	{
	private:
		Object2D::Ptr m_parent;
		Object2D::Ptr m_child;
		ChildManagementMode::Flags m_managementMode;
		ChildTransformingMode m_transformingMode;
		RegistrationCommand m_command;

	public:
		EventToManageFamilyship2D(Object2D::Ptr parent, Object2D::Ptr child)
			: m_parent(parent)
			, m_child(child)
		{
		}

		void SetUpAsAddEvent(
			ChildManagementMode::Flags managementMode,
			ChildTransformingMode transformingMode)
		{
			m_managementMode = managementMode;
			m_transformingMode = transformingMode;
			m_command = RegistrationCommand::Add;
		}

		void SetUpAsRemoveEvent()
		{
			m_command = RegistrationCommand::Remove;
		}

		void Commit()
		{
			switch (m_command)
			{
			case asd::RegistrationCommand::Add:
				m_parent->ImmediatelyAddChild(m_child, m_managementMode, m_transformingMode);
				break;

			case asd::RegistrationCommand::Remove:
				m_parent->ImmediatelyRemoveChild(m_child);
				break;
			}
		}
	};
}