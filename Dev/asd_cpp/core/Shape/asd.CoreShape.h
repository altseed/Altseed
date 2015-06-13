#pragma once

#include "asd.ShapeType.h"
#include "../asd.Core.Base.h"
#include <Culling2D.h>

class b2Shape;

namespace asd{

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
