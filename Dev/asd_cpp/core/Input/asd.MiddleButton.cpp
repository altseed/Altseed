#include "asd.MiddleButton.h"

namespace asd{
	MiddleButton::MiddleButton(ButtonState mouseButtonState, float rotation)
	{
		m_mouseButtonState = mouseButtonState;
		m_rotation = rotation;
	}

	ButtonState MiddleButton::GetButtonState() const
	{
		return m_mouseButtonState;
	}

	float MiddleButton::GetRotation() const
	{
		return m_rotation;
	}
};
