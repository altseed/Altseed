#pragma once

#include "ace.IMouseButtonState.h"

namespace ace{


	class IMouseWheelableButtonState:public IMouseButtonState
	{
	public:

		virtual eMouseButtonState GetButtonState() const = 0;

		virtual const double GetRotation() const = 0;
	};
};