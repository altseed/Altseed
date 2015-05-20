#pragma once

#include "ace.CoreTriangleShape.h"
#include "../ace.ReferenceObject.h"
#include <array>

namespace ace
{
	class CoreTriangleShape_Imp
		:public CoreTriangleShape
		, public ReferenceObject
	{
		std::array<Vector2DF, 3> points;
		std::array<Vector2DF, 3> uvs;

	public:
		CoreTriangleShape_Imp();
		virtual ~CoreTriangleShape_Imp(){}

		Vector2DF GetPointByIndex(int index) const;
		void SetPointByIndex(Vector2DF point, int index);

		Vector2DF GetUVByIndex(int index) const;
		void SetUVByIndex(Vector2DF uv, int index);

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