#pragma once

#include "ace.Joystick.h"

namespace ace{
	class Joystick_Imp:public Joystick
	{
		int m_connectId;

		int m_buttonsCount;
		int m_axesCount;

		static const int MAX_AXES_NUM = 10;
		static const int MAX_BUTTONS_NUM = 30;

		float m_currentAxes[MAX_AXES_NUM];

		bool m_currentButtonHit[MAX_BUTTONS_NUM];
		bool m_preButtonHit[MAX_BUTTONS_NUM];

		ace::astring m_joystickName;

		Joystick_Imp(int id);
	public:
		virtual ~Joystick_Imp();
		static Joystick_Imp* Create(int id);

		void RefreshInputState();

		const ace::achar* GetJoystickName() override;
		int GetButtonsCount() override;
		int GetAxesCount() override;
		JoystickButtonState GetButtonState(int at) override;
		float GetAxisState(int at) override;
	};
};
