#pragma once

#include "ace.CorePolygonShape.h"
#include "../ace.ReferenceObject.h"

namespace ace
{
	class CorePolygonShape_Imp
		:public CorePolygonShape
		, public ReferenceObject
	{
	private:
		std::vector<Vector2DF> vertexes;
	public:
		CorePolygonShape_Imp();
		virtual ~CorePolygonShape_Imp();

		void AddVertex(Vector2DF vertex) override;
		void ClearVertexes() override;
		int GetVertexesNum() const override;

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