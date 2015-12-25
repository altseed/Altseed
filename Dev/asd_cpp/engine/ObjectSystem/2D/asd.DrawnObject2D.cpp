#include "asd.DrawnObject2D.h"

namespace asd
{
	void DrawnObject2D::AddDrawnChild(const DrawnObject2D::Ptr& child,
		ChildManagementMode::Flags managementMode,
		ChildTransformingMode transformingMode,
		ChildDrawingMode::Flags drwingMode)
	{
		GetCoreDrawnObject()->AddChild(child->GetCoreDrawnObject(),
			managementMode,
			transformingMode,
			drwingMode);
		m_children.push_back(child);
	}
}