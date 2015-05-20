#pragma once

#include "ace.JoystickContainer.h"
#include <array>

namespace ace{
	class JoystickContainer_Imp:public JoystickContainer
	{
		static const int MAX_CONTAINER_SIZE = 16;
		std::array<bool, MAX_CONTAINER_SIZE> isPresent;

		Joystick_Imp *m_joystickContainer[MAX_CONTAINER_SIZE];
		JoystickContainer_Imp();
	public:
		virtual ~JoystickContainer_Imp();
		void RefreshJoysticks();
		static JoystickContainer_Imp* Create();
		bool GetIsPresentAt(int at) override;
		Joystick* GetJoystickAt(int at) override;
		void RefreshAllJoysticks() override;
	};
};
