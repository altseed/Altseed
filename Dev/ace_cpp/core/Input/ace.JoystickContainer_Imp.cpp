#include "../ace.Core.Base_Imp.h"
#include "ace.Joystick_Imp.h"
#include "ace.JoystickContainer_Imp.h"
namespace ace{
	JoystickContainer_Imp::JoystickContainer_Imp()
	{
		for (int i = 0; i < MAX_CONTAINER_SIZE; ++i)
		{
			m_joystickContainer[i] = Joystick_Imp::Create(i);
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
		return glfwJoystickPresent(at) == GL_TRUE;
	}
	Joystick* JoystickContainer_Imp::GetJoystickAt(int at)
	{
		return (GetIsPresentAt(at)) ? m_joystickContainer[at] : nullptr;
	}
};
