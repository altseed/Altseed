#pragma once

#include "ace.CoreLineShape.h"
#include "../ace.ReferenceObject.h"

namespace ace
{
	class CoreLineShape_Imp
		:public CoreLineShape
		, public ReferenceObject
	{
		Vector2DF staringPosition;
		Vector2DF endingPosition;
		float thickness;
	public:
		CoreLineShape_Imp();
		virtual ~CoreLineShape_Imp(){}

		Vector2DF GetStartingPosition() const;
		void SetStartingPosition(Vector2DF startingPosition);

		Vector2DF GetEndingPosition() const;
		void SetEndingPosition(Vector2DF endingPosition);

		float GetThickness() const;
		void SetThickness(float thickness);

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
