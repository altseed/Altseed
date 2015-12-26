#pragma once
#include <memory>
#include "asd.Object2D.h"

namespace asd
{
	class DrawnObject2D : public Object2D
	{
	private:
		virtual CoreDrawnObject2D* GetCoreDrawnObject() const = 0;

	public:
		typedef std::shared_ptr<DrawnObject2D> Ptr;

		virtual Color GetColor() const = 0;
		virtual void SetColor(Color value) = 0;

		virtual int GetDrawingPriority() const = 0;
		virtual void SetDrawingPriority(int value) = 0;

		void AddDrawnChild(const DrawnObject2D::Ptr& child,
			ChildManagementMode::Flags managementMode,
			ChildTransformingMode transformingMode,
			ChildDrawingMode::Flags drwingMode);
	};
}