#include "ace.RectI.h"

namespace ace
{
	RectI::RectI(int x, int y, int width, int height)
		: X(x), Y(y), Width(width), Height(height)
	{
	}

	Vector2DI RectI::GetPosition() const
	{
		return Vector2DI(X, Y);
	}

	Vector2DI RectI::GetSize() const
	{
		return Vector2DI(Width, Height);
	}
}