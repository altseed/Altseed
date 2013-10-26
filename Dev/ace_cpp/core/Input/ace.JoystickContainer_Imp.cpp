#include "../ace.Core.Base_Imp.h"
#include "ace.Joystick_Imp.h"
#include "ace.JoystickContainer_Imp.h"
namespace ace{
	JoystickContainer_Imp::JoystickContainer_Imp()
	{
		for (int i = 0; i < MAX_CONTAINER_SIZE; ++i)
		{
			joystickContainer[i] = Joystick_Imp::Create(i);
		}
	}

	JoystickContainer_Imp::~JoystickContainer_Imp()
	{
		for (int i = 0; i < MAX_CONTAINER_SIZE; ++i)
		{
			delete joystickContainer[i];
		}
	}

	void JoystickContainer_Imp::RefreshJoysticks()
	{
		for (int i = 0; i < MAX_CONTAINER_SIZE; ++i)
		{
			if (IsPresentAt(i)) joystickContainer[i]->RefreshInputState();
		}
	}

	JoystickContainer_Imp* JoystickContainer_Imp::Create()
	{
		return new JoystickContainer_Imp();
	}

	const bool JoystickContainer_Imp::IsPresentAt(int at)
	{
		return glfwJoystickPresent(at) == GL_TRUE;
	}
	Joystick* JoystickContainer_Imp::GetJoystickAt(int at)
	{
		return (IsPresentAt(at)) ? joystickContainer[at] : NULL;
	}
};