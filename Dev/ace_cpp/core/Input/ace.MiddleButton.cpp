#include "ace.MiddleButton.h"

namespace ace{
	MiddleButton::MiddleButton(MouseButtonState mouseButtonState, double rotation)
	{
		m_mouseButtonState = mouseButtonState;
		m_rotation = rotation;
	}

	const MouseButtonState MiddleButton::GetButtonState() const
	{
		return m_mouseButtonState;
	}

	const double MiddleButton::GetRotation() const
	{
		return m_rotation;
	}
};