#pragma once
#include "asd.CoreCollider2D.h"
#include "../common/Math/asd.Vector2DF.h"
#include "../common/Math/asd.RectF.h"

namespace asd
{
	class CoreRectangleCollider
		:public CoreCollider2D
	{
	public:
		CoreRectangleCollider() {}
		virtual ~CoreRectangleCollider() {}

		virtual RectF GetArea() const = 0;
		virtual void SetArea(RectF area) = 0;
	};
};