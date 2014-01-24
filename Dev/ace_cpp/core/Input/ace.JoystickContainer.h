#pragma once

#include "ace.Joystick.h"

namespace ace{
	class JoystickContainer
	{
	public:
		JoystickContainer(){}
		virtual ~JoystickContainer(){}

		virtual const bool IsPresentAt(int at) = 0;


		virtual Joystick* GetJoystickAt(int at) = 0;
	};
};
