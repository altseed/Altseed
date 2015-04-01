#pragma once

#include "../ace.CoreToEngine.h"

namespace ace
{
	class GeometryObject2D;

	class Shape
	{
		friend class GeometryObject2D;
	protected:
		virtual std::shared_ptr<CoreShape> GetCoreShape() const = 0;
	public:
		virtual ShapeType GetShapeType() const = 0;
	};
};