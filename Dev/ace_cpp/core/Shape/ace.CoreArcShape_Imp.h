#pragma once

#include "../ace.ReferenceObject.h"
#include "ace.CoreShape_Imp.h"
#include "ace.CoreArcShape.h"

namespace ace
{
	class CoreArcShape_Imp
		:public CoreArcShape
		, public ReferenceObject
		, public CoreShape_Imp
	{
		int startingCorner;
		int endingCorner;
		Vector2DF position;
		float innerDiameter;
		float outerDiameter;
		int numberOfCorners;
		float angle;
	public:
		CoreArcShape_Imp();
		virtual ~CoreArcShape_Imp(){}

		Vector2DF GetPosition() const override;
		void SetPosition(Vector2DF position) override;

		float GetOuterDiameter() const override;
		void SetOuterDiameter(float outerDiameter) override;

		float GetInnerDiameter() const override;
		void SetInnerDiameter(float innerDiameter) override;

		float GetAngle() const override;
		void SetAngle(float angle) override;

		int GetNumberOfCorners() const override;
		void SetNumberOfCorners(int numberOfCorners) override;

		int GetStartingCorner() const override;
		void SetStartingCorner(int startingCorner) override;

		int GetEndingCorner() const override;
		void SetEndingCorner(int endingCorner) override;

		ShapeType GetShapeType() const override;
		ShapeType GetType() const override;

#if !SWIG
	public:
		virtual int GetRef() { return ReferenceObject::GetRef(); }
		virtual int AddRef() { return ReferenceObject::AddRef(); }
		virtual int Release() { return ReferenceObject::Release(); }
	protected:
		virtual void DivideToTriangles() override;
		virtual void CalculateBoundingCircle() override;
		virtual void CalcCollisions() override;
#endif

	public:

		virtual bool GetIsCollidedWith(CoreShape* shape) override
		{
			if (GetType() == ShapeType::RectangleShape)
			{
				if (shape->GetShapeType() == ShapeType::CircleShape)
				{
					return GetIsCollidedWithCircleAndRect((CoreCircleShape*)shape, (CoreRectangleShape*)this);
				}
			}
			else if (GetType() == ShapeType::LineShape)
			{
				if (shape->GetShapeType() == ShapeType::CircleShape)
				{
					return GetIsCollidedWithCircleAndLine((CoreCircleShape*)shape, (CoreLineShape*)this);
				}
			}
			else if (GetType() == ShapeType::CircleShape)
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
	};
};
