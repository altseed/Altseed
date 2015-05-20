#pragma once

#include "ace.CoreShape.h"
#include "../common/Math/ace.Vector2DF.h"

namespace ace
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