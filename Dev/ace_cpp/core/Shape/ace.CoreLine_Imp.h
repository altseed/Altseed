#pragma once

#include "ace.CoreLine.h"
#include "../ace.ReferenceObject.h"

namespace ace
{
	class CoreLine_Imp
		:public CoreLine
		, public ReferenceObject
	{
		Vector2DF staringPosition;
		Vector2DF endingPosition;
		float thickness;
	public:
		CoreLine_Imp();
		virtual ~CoreLine_Imp(){}

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
		virtual void DivideToTriangles() override;
#endif
	};
};
