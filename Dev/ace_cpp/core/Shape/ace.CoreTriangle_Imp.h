#pragma once

#include "ace.CoreTriangle.h"
#include "../ace.ReferenceObject.h"
#include <array>

namespace ace
{
	class CoreTriangle_Imp
		:public CoreTriangle
		, public ReferenceObject
	{
		std::array<Vector2DF, 3> points;
		std::array<Vector2DF, 3> uvs;

	public:
		CoreTriangle_Imp();
		virtual ~CoreTriangle_Imp(){}

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
		virtual void DivideToTriangles() override;
#endif
	};
};