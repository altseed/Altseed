#include "../asd.Core.Base_Imp.h"
#include "asd.Joystick_Imp.h"
#include "asd.JoystickContainer_Imp.h"
namespace asd{
	JoystickContainer_Imp::JoystickContainer_Imp()
	{
		for (int i = 0; i < MAX_CONTAINER_SIZE; ++i)
		{
			m_joystickContainer[i] = Joystick_Imp::Create(i);
			isPresent[i] = glfwJoystickPresent(i) == GL_TRUE;
		}
	}

	JoystickContainer_Imp::~JoystickContainer_Imp()
	{
		for (int i = 0; i < MAX_CONTAINER_SIZE; ++i)
		{
			SafeDelete(m_joystickContainer[i]);
		}
	}

	void JoystickContainer_Imp::RefreshJoysticks()
	{
		for (int i = 0; i < MAX_CONTAINER_SIZE; ++i)
		{
			if (GetIsPresentAt(i)) m_joystickContainer[i]->RefreshInputState();
		}
	}

	JoystickContainer_Imp* JoystickContainer_Imp::Create()
	{
		return new JoystickContainer_Imp();
	}

	bool JoystickContainer_Imp::GetIsPresentAt(int at)
	{
		return isPresent[at];
	}

	Joystick* JoystickContainer_Imp::GetJoystickAt(int at)
	{
		return (GetIsPresentAt(at)) ? m_joystickContainer[at] : nullptr;
	}

	void JoystickContainer_Imp::RefreshAllJoysticks()
	{
		for (int i = 0; i < MAX_CONTAINER_SIZE; ++i)
		{
			isPresent[i] = glfwJoystickPresent(i) == GL_TRUE;
		}
	}
};
