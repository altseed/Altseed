#pragma once

#include "ace.CoreCircle.h"

namespace ace
{
	class CoreArc
		:public CoreShape
	{
	public:
		CoreArc(){}
		virtual ~CoreArc(){}

		virtual Vector2DF GetPosition() const = 0;
		virtual void SetPosition(Vector2DF position) = 0;

		virtual float GetOuterDiameter() const = 0;
		virtual void SetOuterDiameter(float outerDiameter) = 0;

		virtual float GetInnerDiameter() const = 0;
		virtual void SetInnerDiameter(float innerDiameter) = 0;

		virtual float GetAngle() const = 0;
		virtual void SetAngle(float angle) = 0;

		virtual int GetStartingCorner() const = 0;
		virtual void SetStartingCorner(int staringCorner) = 0;

		virtual int GetEndingCorner() const = 0;
		virtual void SetEndingCorner(int endingCorner) = 0;

		virtual int GetNumberOfCorners() const = 0;
		virtual void SetNumberOfCorners(int numberOfCorners) = 0;
	};
};