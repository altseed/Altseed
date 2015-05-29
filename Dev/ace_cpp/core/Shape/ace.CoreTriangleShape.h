#pragma once

#include "ace.CoreShape.h"
#include "../common/Math/ace.Vector2DF.h"

namespace ace
{
	class CoreTriangleShape
		:public CoreShape
	{
	public:
		CoreTriangleShape(){}
		virtual ~CoreTriangleShape(){}

		virtual Vector2DF GetPointByIndex(int index) const = 0;
		virtual void SetPointByIndex(Vector2DF point, int index) = 0;

		virtual Vector2DF GetUVByIndex(int index) const = 0;
		virtual void SetUVByIndex(Vector2DF uv, int index) = 0;

		virtual ShapeType GetShapeType() const = 0;
	};
};