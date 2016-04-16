#include "asd.MiddleButton.h"

namespace asd{
	MiddleButton::MiddleButton(MouseButtonState mouseButtonState, float rotation)
	{
		m_mouseButtonState = mouseButtonState;
		m_rotation = rotation;
	}

	MouseButtonState MiddleButton::GetButtonState() const
	{
		return m_mouseButtonState;
	}

	float MiddleButton::GetRotation() const
	{
		return m_rotation;
	}
};
