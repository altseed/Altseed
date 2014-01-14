#pragma once

#include "../ace.common.Base.h"
#include "ace.Vector2DI.h"

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
	};
}