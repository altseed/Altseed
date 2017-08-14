#pragma once
#include "asd.CoreCollider2D.h"

namespace asd {

	class CoreCircleCollider
		:public CoreCollider2D
	{
	public:

		CoreCircleCollider() {}
		virtual ~CoreCircleCollider() {}

		virtual Vector2DF GetCenter() const = 0;
		virtual void SetCenter(Vector2DF position) = 0;

		virtual float GetRadius() const = 0;
		virtual void SetRadius(float radius) = 0;
	};
}