#pragma once
#include "../common/Graphics/asd.Color.h"
#include "../asd.ChildManagementMode.h"
#include "../asd.ChildTransformingMode.h"
#include "../asd.ChildDrawingMode.h"
#include "asd.CoreObject2D.h"

namespace asd
{
	class CoreDrawnObject2D : public CoreObject2D
	{
	public:
		virtual Color GetColor() const = 0;
		virtual void SetColor(Color value) = 0;

		virtual int GetDrawingPriority() const = 0;
		virtual void SetDrawingPriority(int value) = 0;

		virtual Color GetAbsoluteColor() const = 0;
		virtual int GetAbsoluteDrawingPriority() const = 0;

		virtual void AddDrawnChild(CoreDrawnObject2D* child,
			ChildManagementMode::Flags managementMode,
			ChildTransformingMode transformingMode,
			ChildDrawingMode::Flags drawingMode) = 0;

#if !SWIG
		virtual void SetParent(CoreDrawnObject2D& parent,
			ChildManagementMode::Flags managementMode,
			ChildTransformingMode transformingMode,
			ChildDrawingMode::Flags drawingMode) = 0;
#endif
	};
}