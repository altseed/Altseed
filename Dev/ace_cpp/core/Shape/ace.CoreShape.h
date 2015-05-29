#pragma once

#include "ace.ShapeType.h"
#include "../ace.Core.Base.h"
#include <Culling2D.h>

class b2Shape;

namespace ace{

	class CoreTriangleShape;

	class CoreShape 
		: public IReference
	{
	protected:
		virtual void DivideToTriangles() = 0;

		virtual void CalcCollisions() = 0;

		virtual void CalculateBoundingCircle() = 0;
	public:

		CoreShape() {}
		virtual ~CoreShape(){}

		virtual ShapeType GetShapeType() const = 0;

		virtual bool GetIsCollidedWith(CoreShape* shape) = 0;
	};
};
