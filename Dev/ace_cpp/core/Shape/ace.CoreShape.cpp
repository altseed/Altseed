#include "ace.CoreShape.h"
#include "ace.CoreTriangleShape.h"
#include "ace.CoreLineShape_Imp.h"
#include "ace.CoreRectangleShape_Imp.h"
#include "ace.CoreCircleShape_Imp.h"
#include <Box2D/Box2D.h>

#ifdef _WIN64

#if _DEBUG
#pragma comment(lib,"x64/Debug/Box2D.lib")
#else
#pragma comment(lib,"x64/Release/Box2D.lib")
#endif

#else

#if _DEBUG
#pragma comment(lib,"x86/Debug/Box2D.lib")
#else
#pragma comment(lib,"x86/Release/Box2D.lib")
#endif

#endif

namespace ace
{
	CoreShape::~CoreShape()
	{
		for (auto triangle : triangles)
		{
			SafeRelease(triangle);
		}
	}

	bool CoreShape::GetIsCollidedb2Shapes(CoreShape* shape)
	{
		for (auto selfShape : GetCollisionShapes())
		{
			for (auto theirsShape : shape->GetCollisionShapes())
			{

				b2Transform identity = b2Transform();
				identity.SetIdentity();

				bool isOverlap = b2TestOverlap(selfShape, 1, theirsShape, 1, identity, identity);


				if (isOverlap)
				{
					return true;
				}
			}
		}
	}


	bool CoreShape::GetIsCollidedWithCircleAndRect(const CoreCircleShape* circle, const CoreRectangleShape* rectangle)
	{
		Vector2DF rectGlobalCenter = rectangle->GetDrawingArea().GetPosition() + rectangle->GetDrawingArea().GetSize() / 2;

		Vector2DF c;

		c.X = std::cos(rectangle->GetAngle()) * (circle->GetPosition().X - rectGlobalCenter.X) -
			std::sin(rectangle->GetAngle()) * (circle->GetPosition().Y - rectGlobalCenter.Y) + rectGlobalCenter.X;
		c.Y = std::sin(rectangle->GetAngle()) * (circle->GetPosition().X - rectGlobalCenter.X) +
			std::cos(rectangle->GetAngle()) * (circle->GetPosition().Y - rectGlobalCenter.Y) * rectGlobalCenter.Y;

		Vector2DF nearestPos;

		Vector2DF rectGlobalPos = rectangle->GetDrawingArea().GetPosition();
		Vector2DF rectSize = rectangle->GetDrawingArea().GetSize();

		if (c.X < rectGlobalPos.X)
		{
			nearestPos.X = rectGlobalPos.X;
		}
		else if (c.X > rectGlobalPos.X + rectSize.X)
		{
			nearestPos.X = rectGlobalPos.X + rectSize.X;
		}
		else
		{
			nearestPos.X = c.X;
		}

		if (c.Y < rectGlobalPos.Y)
		{
			nearestPos.Y = rectGlobalPos.Y;
		}
		else if (c.Y > rectGlobalPos.Y + rectSize.Y)
		{
			nearestPos.Y = rectGlobalPos.Y + rectSize.Y;
		}
		else
		{
			nearestPos.Y = c.Y;
		}

		float radius2 = pow(circle->GetOuterDiameter() / 2, 2);

		float dist = (c - nearestPos).GetSquaredLength();

		return dist < radius2;
	}

	bool CoreShape::GetIsCollidedWithCircleAndLine(const CoreCircleShape* circle, const CoreLineShape* line)
	{
		std::shared_ptr<CoreRectangleShape> rect = std::make_shared<CoreRectangleShape_Imp>();

		Vector2DF gravPos = (line->GetStartingPosition() + line->GetEndingPosition()) / 2;
		float halfLen = (line->GetStartingPosition() - gravPos).GetLength();

		rect->SetDrawingArea(RectF(-halfLen + gravPos.X, -line->GetThickness() / 2 + gravPos.Y, halfLen * 2, line->GetThickness()));

		return GetIsCollidedWithCircleAndRect(circle, rect.get());
	}

	bool CoreShape::GetIsCollidedWith(CoreShape* shape)
	{
		if (GetShapeType() == ShapeType::RectangleShape)
		{
			if (shape->GetShapeType() == ShapeType::CircleShape)
			{
				return GetIsCollidedWithCircleAndRect((CoreCircleShape*)shape, (CoreRectangleShape*)this);
			}
		}
		else if (GetShapeType() == ShapeType::LineShape)
		{
			if (shape->GetShapeType() == ShapeType::CircleShape)
			{
				return GetIsCollidedWithCircleAndLine((CoreCircleShape*)shape, (CoreLineShape*)this);
			}
		}
		else if (GetShapeType() == ShapeType::CircleShape)
		{
			if (shape->GetShapeType() == ShapeType::LineShape)
			{
				return GetIsCollidedWithCircleAndLine((CoreCircleShape*)this, (CoreLineShape*)shape);
			}
			else if (shape->GetShapeType() == ShapeType::RectangleShape)
			{
				return GetIsCollidedWithCircleAndRect((CoreCircleShape*)this, (CoreRectangleShape*)shape);
			}
		}
		else
		{
			return GetIsCollidedb2Shapes(shape);
		}
	}

#if !SWIG
	std::vector<CoreTriangleShape*>& CoreShape::GetDividedTriangles()
	{
		if (isNeededUpdating)
		{
			for (auto triangle : triangles)
			{
				SafeRelease(triangle);
			}
			triangles.clear();
			DivideToTriangles();
			isNeededUpdating = false;
		}
		return triangles;
	}

	culling2d::Circle& CoreShape::GetBoundingCircle()
	{
		if (isNeededCalcBoundingCircle)
		{
			CalculateBoundingCircle();
			isNeededCalcBoundingCircle = false;
		}
		return boundingCircle;
	}

	std::vector<b2Shape*>& CoreShape::GetCollisionShapes()
	{
		if (isNeededCalcCollisions)
		{
			for (auto shape : collisionShapes)
			{
				delete shape;
			}
			collisionShapes.clear();
			CalcCollisions();
			isNeededCalcCollisions = false;
		}
		return collisionShapes;
	}
#endif
};