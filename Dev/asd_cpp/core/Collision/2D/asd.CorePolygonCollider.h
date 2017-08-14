#pragma once

#include "asd.CoreCollider2D.h"
#include "../common/Math/asd.Vector2DF.h"

namespace asd {

	class CorePolygonCollider
		:public CoreCollider2D
	{
	public:
		CorePolygonCollider() {}
		virtual ~CorePolygonCollider() {}

		virtual bool IsValid() = 0;
		virtual void AddVertex(Vector2DF vertex) = 0;
		virtual void ClearVertexes() = 0;
		virtual int GetVertexesNum() const = 0;

		virtual void SetVertex(int index, Vector2DF vertex) = 0;
		virtual Vector2DF GetVertex(int index) const = 0;
	};
}
