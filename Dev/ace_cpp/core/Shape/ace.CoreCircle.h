#pragma once

#include "ace.CoreShape.h"
#include "../common/Math/ace.Vector2DF.h"

namespace ace
{
	class CoreCircle
		:public CoreShape
	{
	public:
		CoreCircle(){}
		virtual ~CoreCircle(){}

		virtual Vector2DF GetPosition() const = 0;
		virtual void SetPosition(Vector2DF position) = 0;

		virtual float GetRadius() const = 0;
		virtual void SetRadius(float radius) = 0;

		virtual int GetVerticalAngleNum() const = 0;
		virtual void SetVerticalAngleNum(int verticalangleNum) = 0;
	};
};