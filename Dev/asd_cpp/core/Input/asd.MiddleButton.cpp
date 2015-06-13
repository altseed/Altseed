#include "asd.MiddleButton.h"

namespace asd{
	MiddleButton::MiddleButton(MouseButtonState mouseButtonState, double rotation)
	{
		m_mouseButtonState = mouseButtonState;
		m_rotation = rotation;
	}

	MouseButtonState MiddleButton::GetButtonState() const
	{
		return m_mouseButtonState;
	}

	double MiddleButton::GetRotation() const
	{
		return m_rotation;
	}
};
