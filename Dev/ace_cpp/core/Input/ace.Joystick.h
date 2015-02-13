#pragma once

namespace ace{

	enum class JoystickButtonState:int
	{
		Push,
		Release,
		Free,
		Hold
	};

	class Joystick
	{
	public:
		Joystick(){}
		virtual ~Joystick(){}

		virtual const ace::achar* GetJoystickName() = 0;


		virtual const int GetButtonsCount() = 0;


		virtual const int GetAxesCount() = 0;


		virtual const JoystickButtonState GetButtonState(int at) = 0;


		virtual const float GetAxisState(int at) = 0;
	};
};
