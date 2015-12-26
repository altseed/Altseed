#pragma once
#include "asd.Object2D.h"
#include "../../asd.CoreToEngine.h"

namespace asd
{
	class ParentInfo2D
	{
	private:
		Object2D::Ptr m_parent;
		ChildManagementMode::Flags m_managementMode;

	public:
		const Object2D::Ptr& GetParent() const
		{
			return m_parent;
		}

		ChildManagementMode::Flags GetManagementMode() const
		{
			return m_managementMode;
		}
	};
}