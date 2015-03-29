#pragma once

#include "ace.CoreLine.h"

namespace ace
{
	class CoreLine_Imp
		:public CoreLine
		, public ReferenceObject
	{
		Vector2DF staringPoint;
		Vector2DF endingPoint;
		float thickness;
	public:
		CoreLine_Imp();
		virtual ~CoreLine_Imp(){}

		Vector2DF GetStartingPoint() const;
		void SetStartingPoint(Vector2DF startingPoint);

		Vector2DF GetEndingPoint() const;
		void SetEndingPoint(Vector2DF endingPoint);

		float GetThickness() const;
		void SetThickness(float thickness);

		ShapeType GetShapeType() const override;

#if !SWIG
	public:
		virtual int GetRef() { return ReferenceObject::GetRef(); }
		virtual int AddRef() { return ReferenceObject::AddRef(); }
		virtual int Release() { return ReferenceObject::Release(); }
		virtual std::vector<CoreTriangle*> GetDividedTriangles() override;
#endif
	};
};
