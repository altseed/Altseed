#pragma once

#include "../ace.ReferenceObject.h"
#include "ace.CoreArc.h"

namespace ace
{
	class CoreArc_Imp
		:public CoreArc
		, public ReferenceObject
	{
		int startingCorner;
		int endingCorner;
		Vector2DF position;
		float innerDiameter;
		float outerDiameter;
		int numberOfCorners;
		float angle;
	public:
		CoreArc_Imp();
		virtual ~CoreArc_Imp(){}

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

		int GetStartingCorner() const override;
		void SetStartingCorner(int startingCorner) override;

		int GetEndingCorner() const override;
		void SetEndingCorner(int endingCorner) override;

		ShapeType GetShapeType() const override;

#if !SWIG
	public:
		virtual int GetRef() { return ReferenceObject::GetRef(); }
		virtual int AddRef() { return ReferenceObject::AddRef(); }
		virtual int Release() { return ReferenceObject::Release(); }
	protected:
		virtual void DivideToTriangles() override;
		virtual void CalculateBoundingCircle() override;
#endif
	};
};
