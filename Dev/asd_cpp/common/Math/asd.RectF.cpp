
#include "asd.RectF.h"
#include "asd.RectI.h"
#include "asd.Vector2DF.h"

using namespace std;

namespace asd
{
	RectF::RectF()
		: X(0), Y(0), Width(0), Height(0)
	{
	}

	RectF::RectF(float x, float y, float width, float height)
		: X(x), Y(y), Width(width), Height(height)
	{
	}

	RectF::RectF(Vector2DF position, Vector2DF size)
		: X(position.X), Y(position.Y), Width(size.X), Height(size.Y)
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

	bool RectF::operator!=(const RectF& right)
	{
		return
			X != right.X ||
			Y != right.Y ||
			Width != right.Width ||
			Height != right.Height;
	}

	RectI RectF::ToI() const
	{
		return RectI((int32_t) X, (int32_t) Y, (int32_t) Width, (int32_t) Height);
	}
}