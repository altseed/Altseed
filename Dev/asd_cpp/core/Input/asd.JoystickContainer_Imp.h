#pragma once

#include "asd.JoystickContainer.h"

#include "../Window/asd.Window_Imp.h"

#include <array>

namespace asd
{
	class JoystickContainer_Imp
		: public JoystickContainer
	{
		Window* window = nullptr;
		ap::Joystick* joystick = nullptr;

		static const int MAX_CONTAINER_SIZE = 16;
		Joystick_Imp* m_joystickContainer[MAX_CONTAINER_SIZE];

		JoystickContainer_Imp(Window* window);
	public:
		virtual ~JoystickContainer_Imp();
		void RefreshJoysticks();
		static JoystickContainer_Imp* Create(Window* window);
		bool GetIsPresentAt(int at) override;
		Joystick* GetJoystickAt(int at) override;
		void RefreshAllJoysticks() override;
	};
};
