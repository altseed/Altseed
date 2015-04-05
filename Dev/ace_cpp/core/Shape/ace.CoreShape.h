#pragma once

#include "ace.ShapeType.h"
#include "../ace.Core.Base.h"
#include <Culling2D.h>

namespace ace{

	class CoreTriangleShape;

	class CoreShape 
		: public IReference
	{
#if !SWIG
	protected:
		std::vector<CoreTriangleShape*> triangles;

		virtual void DivideToTriangles() = 0;

		culling2d::Circle boundingCircle;

		virtual void CalculateBoundingCircle() = 0;
#endif
	protected:
		bool isNeededUpdating;
		bool isNeededCalcBoundingCircle;

	public:

		CoreShape() :
			isNeededUpdating(false),
			isNeededCalcBoundingCircle(false)
		{}
		virtual ~CoreShape();

		virtual ShapeType GetShapeType() const = 0;

#if !SWIG
		std::vector<CoreTriangleShape*>& GetDividedTriangles();

		culling2d::Circle& GetBoundingCircle();
#endif
	};
};
