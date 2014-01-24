#pragma once

#include "ace.JoystickContainer.h"

namespace ace{
	class JoystickContainer_Imp:public JoystickContainer
	{
		static const int MAX_CONTAINER_SIZE = 16;

		Joystick_Imp *joystickContainer[MAX_CONTAINER_SIZE];
		JoystickContainer_Imp();
	public:
		virtual ~JoystickContainer_Imp();
		void RefreshJoysticks();
		static JoystickContainer_Imp* Create();
		const bool IsPresentAt(int at);
		Joystick* GetJoystickAt(int at);
	};
};
