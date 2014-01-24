

#include "../ace.Core.Base_Imp.h"
#include "ace.Joystick.h"
#include "ace.Joystick_Imp.h"
namespace ace{

	Joystick_Imp::Joystick_Imp(int id)
	{
		connectId = id;
		buttonsCount = 0;
		AxesCount = 0;
		joystickName = ToAString("");

		for (int i = 0; i < MAX_BUTTONS_NUM; ++i)
		{
			currentButtonHit[i] = false;
			preButtonHit[i] = false;
		}

		for (int i = 0; i < MAX_AXES_NUM; ++i)
		{
			currentAxes[i] = 0;
		}
	}

	Joystick_Imp* Joystick_Imp::Create(int id)
	{
		return new Joystick_Imp(id);
	}

	Joystick_Imp::~Joystick_Imp()
	{

	}

	void Joystick_Imp::RefreshInputState()
	{

		const float* ax=glfwGetJoystickAxes(connectId, &AxesCount);
		for (int i = 0; i < AxesCount; ++i)
		{
			currentAxes[i] = ax[i];
		}

		const unsigned char* btns = glfwGetJoystickButtons(connectId, &buttonsCount);
		for (int i = 0; i < buttonsCount; ++i)
		{
			preButtonHit[i] = currentButtonHit[i];
			currentButtonHit[i] = btns[i];
		}
	}

	const ace::achar* Joystick_Imp::GetJoystickName()
	{

		joystickName=ToAString(glfwGetJoystickName(connectId));
		return joystickName.c_str();
	}


	const int Joystick_Imp::GetButtonsCount()
	{
		return buttonsCount;
	}


	const int Joystick_Imp::GetAxesCount()
	{
		return AxesCount;
	}


	const eJoystickButtonState Joystick_Imp::GetButtonState(int at)
	{
		if (currentButtonHit[at] && preButtonHit[at]) return ACE_JOYSTICK_BUTTON_HOLD;
		else if (currentButtonHit[at] && !preButtonHit[at]) return ACE_JOYSTICK_BUTTON_PUSH;
		else if (!currentButtonHit[at] && preButtonHit[at]) return ACE_JOYSTICK_BUTTON_PULL;
		else return ACE_JOYSTICK_BUTTON_FREE;
	}


	const float Joystick_Imp::GetAxisState(int at)
	{
		return currentButtonHit[at];
	}
};
