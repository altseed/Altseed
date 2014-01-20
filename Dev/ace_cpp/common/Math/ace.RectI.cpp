#include "ace.RectI.h"

using namespace std;

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

	array<Vector2DF, 4> RectI::GetVertexes() const
	{
		array<Vector2DF, 4> result =
		{
			Vector2DF(X, Y),
			Vector2DF(X + Width, Y),
			Vector2DF(X + Width, Y + Height),
			Vector2DF(X, Y + Height)
		};
		return result;
	}
}