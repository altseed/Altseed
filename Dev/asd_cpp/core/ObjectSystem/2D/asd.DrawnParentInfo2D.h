#pragma once
#include "asd.ParentInfo2D.h"
#include "../asd.ChildManagementMode.h"
#include "../asd.ChildTransformingMode.h"
#include "../asd.ChildDrawingMode.h"
#include "asd.DrawnObject2D.h"

namespace asd
{
	class DrawnParentInfo2D : public ParentInfo2D
	{
	protected:
		DrawnObject2D* m_drawnParent;
		ChildDrawingMode::Flags m_drawingMode;

	public:
		DrawnParentInfo2D(DrawnObject2D* parent,
			ChildManagementMode::Flags managementMode,
			ChildTransformingMode transformingMode,
			ChildDrawingMode::Flags drawingMode)
			: ParentInfo2D(parent, managementMode, transformingMode)
			, m_drawingMode(drawingMode)
			, m_drawnParent(parent)
		{
		}

		Color GetInheritedColor() const
		{
			return (m_drawingMode & ChildDrawingMode::Color != 0) ? m_drawnParent->GetColor() : Color(255, 255, 255, 255);
		}

		int GetInheritedDrawingPriority() const
		{
			return (m_drawingMode & ChildDrawingMode::DrawingPriority != 0) ? m_drawnParent->GetDrawingPriority() : 0;
		}
	};
}