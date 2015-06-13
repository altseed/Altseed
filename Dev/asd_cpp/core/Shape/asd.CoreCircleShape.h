#pragma once

#include "asd.CoreShape.h"
#include "../common/Math/asd.Vector2DF.h"

namespace asd
{
	class CoreCircleShape
		:public CoreShape
	{
	public:
		CoreCircleShape(){}
		virtual ~CoreCircleShape(){}

		virtual Vector2DF GetPosition() const = 0;
		virtual void SetPosition(Vector2DF position) = 0;

		virtual float GetOuterDiameter() const = 0;
		virtual void SetOuterDiameter(float outerDiameter) = 0;

		virtual float GetInnerDiameter() const = 0;
		virtual void SetInnerDiameter(float innerDiameter) = 0;

		virtual float GetAngle() const = 0;
		virtual void SetAngle(float angle) = 0;

		virtual int GetNumberOfCorners() const = 0;
		virtual void SetNumberOfCorners(int numberOfCorners) = 0;
	};
};