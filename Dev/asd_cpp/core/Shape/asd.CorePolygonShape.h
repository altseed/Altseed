﻿#pragma once

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

		virtual bool IsValid() = 0;
		virtual void AddVertex(Vector2DF vertex) = 0;
		virtual void ClearVertexes() = 0;
		virtual int GetVertexesNum() const = 0;

		virtual bool AddHole(CoreShape *holeShape) = 0;
		virtual bool RemoveHole(CoreShape *holeShape) = 0;
	};
};