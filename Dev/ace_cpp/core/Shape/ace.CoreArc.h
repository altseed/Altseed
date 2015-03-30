#pragma once

#include "ace.CoreCircle.h"

namespace ace
{
	class CoreArc
		:public CoreCircle
	{
	public:
		CoreArc(){}
		virtual ~CoreArc(){}

		virtual int GetStaringVerticalAngle() const = 0;
		virtual void SetStaringVerticalAngle(int staringverticalAngle) = 0;

		virtual int GetEndingVerticalAngle() const = 0;
		virtual void SetEndingVerticalAngle(int endingverticalAngle) = 0;

	};
};