#pragma once

namespace ace{

	enum eJoystickButtonState
	{
		ACE_JOYSTICK_BUTTON_PUSH,
		ACE_JOYSTICK_BUTTON_PULL,
		ACE_JOYSTICK_BUTTON_FREE,
		ACE_JOYSTICK_BUTTON_HOLD
	};

	enum eJoystickButtons
	{
		BUTTON_1
	};

	enum eJoystickAxises
	{
		AXIS_1
	};

	class Joystick
	{
	public:
		Joystick(){}
		virtual ~Joystick(){}

		virtual const ace::achar* GetJoystickName() = 0;


		virtual const int GetButtonsCount() = 0;


		virtual const int GetAxesCount() = 0;


		virtual const eJoystickButtonState GetButtonState(int at) = 0;


		virtual const float GetAxisState(int at) = 0;
	};
};
