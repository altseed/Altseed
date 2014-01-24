#pragma once

#include "ace.Joystick.h"

namespace ace{
	class Joystick_Imp:public Joystick
	{
		int connectId;

		int buttonsCount;
		int AxesCount;

		static const int MAX_AXES_NUM = 10;
		static const int MAX_BUTTONS_NUM = 30;

		float currentAxes[MAX_AXES_NUM];

		bool currentButtonHit[MAX_BUTTONS_NUM];
		bool preButtonHit[MAX_BUTTONS_NUM];

		ace::astring joystickName;

		Joystick_Imp(int id);
	public:
		virtual ~Joystick_Imp();
		static Joystick_Imp* Create(int id);

		void RefreshInputState();


		const ace::achar* GetJoystickName();
		const int GetButtonsCount();
		const int GetAxesCount();
		const eJoystickButtonState GetButtonState(int at);
		const float GetAxisState(int at);
	};
};
