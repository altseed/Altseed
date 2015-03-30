#pragma once

#include "ace.CoreCircle.h"

namespace ace
{
	class CoreCircle_Imp
		:public CoreCircle
		, public ReferenceObject
	{
		Vector2DF position;
		float radius;
		int verticalAngleNum;
	public:
		CoreCircle_Imp();
		virtual ~CoreCircle_Imp(){}

		Vector2DF GetPosition() const;
		void SetPosition(Vector2DF position);

		float GetRadius() const;
		void SetRadius(float radius);

		int GetVertexNum() const;
		void SetVertexNum(int vertexNum);

		int GetVerticalAngleNum() const;
		void SetVerticalAngleNum(int verticalangleNum);

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