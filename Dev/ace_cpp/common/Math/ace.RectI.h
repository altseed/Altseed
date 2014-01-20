#pragma once

#include <array>
#include "../ace.common.Base.h"
#include "ace.Vector2DI.h"
#include "ace.Vector2DF.h"

namespace ace
{
	struct RectI
	{
	public:
		int X;
		int Y;
		int Width;
		int Height;

		RectI(int x, int y, int width, int height);

		Vector2DI GetPosition() const;

		Vector2DI GetSize() const;

		std::array<Vector2DF, 4> GetVertexes() const;
	};
}