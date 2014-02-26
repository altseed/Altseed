
#include "ace.RectF.h"

using namespace std;

namespace ace
{
	RectF::RectF()
		: X(0), Y(0), Width(0), Height(0)
	{
	}

	RectF::RectF(float x, float y, float width, float height)
		: X(x), Y(y), Width(width), Height(height)
	{
	}

	Vector2DF RectF::GetPosition() const
	{
		return Vector2DF(X, Y);
	}

	Vector2DF RectF::GetSize() const
	{
		return Vector2DF(Width, Height);
	}

	array<Vector2DF, 4> RectF::GetVertexes() const
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