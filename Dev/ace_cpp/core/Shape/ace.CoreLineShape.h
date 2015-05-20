#pragma once

#include "ace.CoreShape.h"
#include "../common/Math/ace.Vector2DF.h"

namespace ace
{
	class CoreLineShape
		:public CoreShape
	{
	public:
		CoreLineShape(){}
		virtual ~CoreLineShape(){}

		virtual Vector2DF GetStartingPosition() const = 0;
		virtual void SetStartingPosition(Vector2DF staringPosition) = 0;

		virtual Vector2DF GetEndingPosition() const = 0;
		virtual void SetEndingPosition(Vector2DF endingPosition) = 0;

		virtual float GetThickness() const = 0;
		virtual void SetThickness(float thickness) = 0;
	};
};