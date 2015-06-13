#pragma once

#include "asd.CoreRectangleShape.h"
#include "asd.CoreShape_Imp.h"
#include "../asd.ReferenceObject.h"

namespace asd
{
	class CoreRectangleShape_Imp
		:public CoreRectangleShape
		, public CoreShape_Imp
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
#include "asd.CoreShape_Imp_Methods.h"
	};
};