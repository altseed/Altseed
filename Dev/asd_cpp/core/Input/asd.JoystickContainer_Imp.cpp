#include "../asd.Core.Base_Imp.h"
#include "asd.Joystick_Imp.h"
#include "asd.JoystickContainer_Imp.h"
namespace asd{
	JoystickContainer_Imp::JoystickContainer_Imp(Window* window)
	{
		this->window = window;
		this->joystick = ap::Joystick::Create(((Window_Imp*)window)->GetWindow());
		SafeAddRef(this->window);

		for (int i = 0; i < MAX_CONTAINER_SIZE; i++)
		{
			m_joystickContainer[i] = Joystick_Imp::Create(i, joystick);
		}
	}

	JoystickContainer_Imp::~JoystickContainer_Imp()
	{
		for (int i = 0; i < MAX_CONTAINER_SIZE; i++)
		{
			SafeDelete(m_joystickContainer[i]);
		}

		SafeDelete(joystick);
		SafeRelease(window);
	}

	void JoystickContainer_Imp::RefreshJoysticks()
	{
		assert(joystick != nullptr);
		joystick->RefreshInputState();
	}

	JoystickContainer_Imp* JoystickContainer_Imp::Create(Window* window)
	{
		return new JoystickContainer_Imp(window);
	}

	bool JoystickContainer_Imp::GetIsPresentAt(int at)
	{
		assert(joystick != nullptr);
		return joystick->IsPresent(at);
	}

	Joystick* JoystickContainer_Imp::GetJoystickAt(int at)
	{
		return (GetIsPresentAt(at)) ? m_joystickContainer[at] : nullptr;
	}

	void JoystickContainer_Imp::RefreshAllJoysticks()
	{
		joystick->RefreshConnectedState();
	}
};
