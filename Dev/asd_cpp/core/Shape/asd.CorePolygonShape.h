#pragma once

#include "asd.CoreShape.h"
#include "../common/Math/asd.Vector2DF.h"

namespace asd
{
	class CorePolygonShape
		:public CoreShape
	{
	public:
		CorePolygonShape(){}
		virtual ~CorePolygonShape(){}

		virtual void AddVertex(Vector2DF vertex) = 0;
		virtual void ClearVertexes() = 0;
		virtual int GetVertexesNum() const = 0;
	};
};