#pragma once

#include "ace.CoreRectangleShape.h"
#include "../ace.ReferenceObject.h"

namespace ace
{
	class CoreRectangleShape_Imp
		:public CoreRectangleShape
		, public ReferenceObject
	{
		RectF drawingArea;
		RectF uv;
		float angle;
		Vector2DF centerPosition;
	public:
		CoreRectangleShape_Imp();
		virtual ~CoreRectangleShape_Imp(){}

		RectF GetDrawingArea() const override;
		void SetDrawingArea(RectF drawingArea) override;

		RectF GetUV() const override;
		void SetUV(RectF uv) override;

		float GetAngle() const override;
		void SetAngle(float angle) override;

		Vector2DF GetCenterPosition() const override;
		void SetCenterPosition(Vector2DF centerPosition) override;

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