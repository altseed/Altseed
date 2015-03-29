#pragma once

#include "ace.CoreShape.h"
#include "../common/Math/ace.Vector2DF.h"

namespace ace
{
	class CoreLine
		:public CoreShape
	{
	public:
		CoreLine(){}
		virtual ~CoreLine(){}

		virtual Vector2DF GetStartingPoint() const = 0;
		virtual void SetStartingPoint(Vector2DF staringPoint) = 0;

		virtual Vector2DF GetEndingPoint() const = 0;
		virtual void SetEndingPoint(Vector2DF endingPoint) = 0;

		virtual float GetThickness() const = 0;
		virtual void SetThickness(float thickness) = 0;
	};
};