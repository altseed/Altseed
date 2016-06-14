#pragma once

#include "asd.CorePolygonShape.h"
#include "../asd.ReferenceObject.h"
#include "asd.CoreShape_Imp.h"

namespace asd
{
	class CorePolygonShape_Imp
		:public CorePolygonShape
		, public ReferenceObject
		, public CoreShape_Imp
	{
	private:
		std::vector<Vector2DF> vertexes;
		std::vector<CoreShape*> holeShapes;
		std::vector<CoreShape_Imp*> holeShapes_Imp;
		bool	isValid = true;

	public:
		CorePolygonShape_Imp();
		virtual ~CorePolygonShape_Imp();

		bool IsValid() override;
		void AddVertex(Vector2DF vertex) override;
		void ClearVertexes() override;
		int GetVertexesNum() const override;

		ShapeType GetShapeType() const override;
		ShapeType GetType() const override;


		bool AddHole(CoreShape *holeShape) override;
		bool RemoveHole(CoreShape *holeShape) override;

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