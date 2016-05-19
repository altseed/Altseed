#pragma once
#include "asd.ICommitable.h"
#include "asd.RegistrationCommand.h"
#include "../2D/asd.DrawnObject2D.h"

namespace asd
{
	class EventToManageDrawnFamilyship2D : public ICommitable
	{
	private:
		DrawnObject2D::Ptr m_parent;
		DrawnObject2D::Ptr m_child;
		ChildManagementMode::Flags m_managementMode;
		ChildTransformingMode m_transformingMode;
		ChildDrawingMode::Flags m_drawingMode;
		RegistrationCommand m_command;

	public:
		EventToManageDrawnFamilyship2D(DrawnObject2D::Ptr parent, DrawnObject2D::Ptr child)
			: m_parent(parent)
			, m_child(child)
		{
		}

		void SetUpAsAddEvent(
			ChildManagementMode::Flags managementMode,
			ChildTransformingMode transformingMode,
			ChildDrawingMode::Flags drawingMode)
		{
			m_managementMode = managementMode;
			m_transformingMode = transformingMode;
			m_drawingMode = drawingMode;
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
				m_parent->ImmediatelyAddDrawnChild(m_child, m_managementMode, m_transformingMode, m_drawingMode);
				break;

			case asd::RegistrationCommand::Remove:
				m_parent->ImmediatelyRemoveChild(m_child);
				break;
			}
		}
	};
}