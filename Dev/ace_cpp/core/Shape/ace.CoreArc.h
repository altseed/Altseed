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

		virtual int GetStartingCorner() const = 0;
		virtual void SetStartingCorner(int staringCorner) = 0;

		virtual int GetEndingCorner() const = 0;
		virtual void SetEndingCorner(int endingCorner) = 0;

	};
};