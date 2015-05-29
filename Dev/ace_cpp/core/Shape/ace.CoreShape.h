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
	private:
		bool GetIsCollidedb2Shapes(CoreShape* shape);
		bool GetIsCollidedWithCircleAndRect(CoreCircleShape* circle, CoreRectangleShape* rectangle);
		bool GetIsCollidedWithCircleAndLine(CoreCircleShape* circle, CoreLineShape* line);

	protected:
		std::vector<CoreTriangleShape*> triangles;
		std::vector<b2Shape*> collisionShapes;

		virtual void DivideToTriangles() = 0;

		virtual void CalcCollisions() = 0;

		culling2d::Circle boundingCircle;

		virtual void CalculateBoundingCircle() = 0;
	protected:
		bool isNeededUpdating;
		bool isNeededCalcBoundingCircle;
		bool isNeededCalcCollisions;

	public:

		CoreShape() :
			isNeededUpdating(false)
			,isNeededCalcBoundingCircle(false)
			, isNeededCalcCollisions(false)
		{}
		virtual ~CoreShape();

		virtual ShapeType GetShapeType() const = 0;

		bool GetIsCollidedWith(CoreShape* shape);

#if !SWIG
		std::vector<CoreTriangleShape*>& GetDividedTriangles();

		std::vector<b2Shape*>& GetCollisionShapes();

		culling2d::Circle& GetBoundingCircle();
#endif
	};
};
