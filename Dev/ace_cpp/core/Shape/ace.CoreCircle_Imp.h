#pragma once

#include "../ace.ReferenceObject.h"
#include "ace.CoreCircle.h"

namespace ace
{
	class CoreCircle_Imp
		:public CoreCircle
		, public ReferenceObject
	{
		Vector2DF position;
		float innerDiameter;
		float outerDiameter;
		int numberOfCorners;
		float angle;
	public:
		CoreCircle_Imp();
		virtual ~CoreCircle_Imp(){}

		Vector2DF GetPosition() const override;
		void SetPosition(Vector2DF position) override;

		float GetOuterDiameter() const override;
		void SetOuterDiamater(float outerDiameter) override;

		float GetInnerDiameter() const override;
		void SetInnerDiamater(float innerDiameter) override;

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
		virtual void DivideToTriangles() override;
#endif
	};
};