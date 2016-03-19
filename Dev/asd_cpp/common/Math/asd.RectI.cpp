
#include "asd.RectF.h"
#include "asd.RectI.h"
#include "asd.Vector2DI.h"

using namespace std;

namespace asd
{
	RectI::RectI()
		: X(0), Y(0), Width(0), Height(0)
	{
	}

	RectI::RectI(int x, int y, int width, int height)
		: X(x), Y(y), Width(width), Height(height)
	{
	}

	RectI::RectI(Vector2DI position, Vector2DI size)
		: X(position.X), Y(position.Y), Width(size.X), Height(size.Y)
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

	array<Vector2DI, 4> RectI::GetVertexes() const
	{
		array<Vector2DI, 4> ret;

		ret[0] = Vector2DI(X, Y);
		ret[1] = Vector2DI(X + Width, Y);
		ret[2] = Vector2DI(X + Width, Y + Height);
		ret[3] = Vector2DI(X, Y + Height);
		
		return ret;
	}

	bool RectI::operator==(const RectI& other) const
	{
		return X == other.X
			&& Y == other.Y
			&& Width == other.Width
			&& Height == other.Height;
	}

	RectF RectI::ToF() const
	{
		return RectF(X, Y, Width, Height);
	}
}