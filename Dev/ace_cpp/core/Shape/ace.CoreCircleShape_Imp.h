#pragma once

#include "../ace.ReferenceObject.h"
#include "ace.CoreCircleShape.h"

namespace ace
{
	class CoreCircleShape_Imp
		:public CoreCircleShape
		, public ReferenceObject
	{
		Vector2DF position;
		float innerDiameter;
		float outerDiameter;
		int numberOfCorners;
		float angle;
	public:
		CoreCircleShape_Imp();
		virtual ~CoreCircleShape_Imp(){}

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

		ShapeType GetShapeType() const override;

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
	};
};