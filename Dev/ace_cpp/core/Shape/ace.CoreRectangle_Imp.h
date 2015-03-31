#pragma once

#include "ace.CoreRectangle.h"
#include "../ace.ReferenceObject.h"

namespace ace
{
	class CoreRectangle_Imp
		:public CoreRectangle
		, public ReferenceObject
	{
		RectF drawingArea;
		RectF uv;
		float angle;
		Vector2DF centerPosition;
	public:
		CoreRectangle_Imp();
		virtual ~CoreRectangle_Imp(){}

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
		virtual void DivideToTriangles() override;
#endif
	};
};