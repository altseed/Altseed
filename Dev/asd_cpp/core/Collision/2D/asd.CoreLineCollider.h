#pragma once
#include "asd.CoreCollider2D.h"
#include "../common/Math/asd.Vector2DF.h"

namespace asd
{
	class CoreLineCollider
		:public CoreCollider2D
	{
	public:
		CoreLineCollider() {}
		virtual ~CoreLineCollider() {}

		virtual Vector2DF GetStartingPosition() const = 0;
		virtual void SetStartingPosition(Vector2DF staringPosition) = 0;

		virtual Vector2DF GetEndingPosition() const = 0;
		virtual void SetEndingPosition(Vector2DF endingPosition) = 0;

		virtual float GetThickness() const = 0;
		virtual void SetThickness(float thickness) = 0;
	};
};