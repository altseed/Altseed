#include "asd.DrawnObject2D.h"
#include "../../asd.CoreToEngine.h"
#include "../Registration/asd.EventToManageDrawnFamilyship2D.h"
using namespace std;

namespace asd
{
	void DrawnObject2D::AddDrawnChild(
		const DrawnObject2D::Ptr& child,
		ChildManagementMode::Flags managementMode,
		ChildTransformingMode transformingMode,
		ChildDrawingMode::Flags drawingMode)
	{
		auto thisPtr = static_pointer_cast<DrawnObject2D>(shared_from_this());
		auto e = make_shared<EventToManageDrawnFamilyship2D>(thisPtr, child);
		e->SetUpAsAddEvent(managementMode, transformingMode, drawingMode);
		Engine::m_changesToCommit.push(e);

		child->m_parentInfo = std::make_shared<ParentInfo2D>(this, managementMode);
		SyncContainerWithChild(child);
	}

	void DrawnObject2D::ImmediatelyAddDrawnChild(
		const DrawnObject2D::Ptr& child,
		ChildManagementMode::Flags managementMode,
		ChildTransformingMode transformingMode,
		ChildDrawingMode::Flags drawingMode)
	{
		GetCoreDrawnObject()->AddDrawnChild(child->GetCoreDrawnObject(),
			managementMode,
			transformingMode,
			drawingMode);
		m_children.push_back(child);
		//child->m_parentInfo = std::make_shared<ParentInfo2D>(this, managementMode);
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