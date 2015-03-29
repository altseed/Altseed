#pragma once

#include "ace.CoreArc.h"

namespace ace
{
	class CoreArc_Imp
		:public CoreArc
		, public ReferenceObject
	{
		int staringVerticalAngle;
		int endingVerticalAngle;
		Vector2DF position;
		float radius;
		int verticalAngleNum;
	public:
		CoreArc_Imp();
		virtual ~CoreArc_Imp(){}

		Vector2DF GetPosition() const override;
		void SetPosition(Vector2DF pos) override;

		float GetRadius() const override;
		void SetRadius(float rad) override;

		int GetVerticalAngleNum() const override;
		void SetVerticalAngleNum(int verticalangleNum) override;

		int GetStaringVerticalAngle() const override;
		void SetStaringVerticalAngle(int staringverticalAngle) override;

		int GetEndingVerticalAngle() const override;
		void SetEndingVerticalAngle(int endingverticalAngle) override;

		ShapeType GetShapeType() const override;

#if !SWIG
	public:
		virtual int GetRef() { return ReferenceObject::GetRef(); }
		virtual int AddRef() { return ReferenceObject::AddRef(); }
		virtual int Release() { return ReferenceObject::Release(); }
#endif
	};
};
