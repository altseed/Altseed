#pragma once

#include "ace.ShapeType.h"
#include "../ace.Core.Base.h"
#include <Culling2D.h>
#include <Box2D/Box2D.h>

namespace ace{

	class CoreTriangleShape;

	class CoreShape 
		: public IReference
	{
	private:
		bool GetIsCollidedb2Shapes(CoreShape* shape);

#if !SWIG
		bool GetIsCollidedWithCircleAndRect(const CoreCircleShape* circle, const CoreRectangleShape* rectangle);
		bool GetIsCollidedWithCircleAndLine(const CoreCircleShape* circle, const CoreLineShape* line);
	protected:
		std::vector<CoreTriangleShape*> triangles;
		std::vector<b2Shape*> collisionShapes;

		virtual void DivideToTriangles() = 0;

		virtual void CalcCollisions() = 0;

		culling2d::Circle boundingCircle;

		virtual void CalculateBoundingCircle() = 0;
#endif
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
