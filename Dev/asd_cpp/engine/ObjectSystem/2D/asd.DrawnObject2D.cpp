#include "asd.DrawnObject2D.h"

namespace asd
{
	void DrawnObject2D::AddDrawnChild(const DrawnObject2D::Ptr& child,
		ChildManagementMode::Flags managementMode,
		ChildTransformingMode transformingMode,
		ChildDrawingMode::Flags drwingMode)
	{
		GetCoreDrawnObject()->AddDrawnChild(child->GetCoreDrawnObject(),
			managementMode,
			transformingMode,
			drwingMode);
		m_children.push_back(child);
		child->m_parentInfo = std::make_shared<ParentInfo2D>(this, managementMode);
	}

	Color DrawnObject2D::GetColor() const
	{
		return GetCoreDrawnObject()->GetColor();
	}

	void DrawnObject2D::SetColor(Color value)
	{
		GetCoreDrawnObject()->SetColor(value);
	}

	int DrawnObject2D::GetDrawingPriority() const
	{
		return GetCoreDrawnObject()->GetDrawingPriority();
	}

	void DrawnObject2D::SetDrawingPriority(int value)
	{
		GetCoreDrawnObject()->SetDrawingPriority(value);
	}

	Color DrawnObject2D::GetAbsoluteColor() const
	{
		return GetCoreDrawnObject()->GetAbsoluteColor();
	}

	int DrawnObject2D::GetAbsoluteDrawingPriority() const
	{
		return GetCoreDrawnObject()->GetAbsoluteDrawingPriority();
	}
}