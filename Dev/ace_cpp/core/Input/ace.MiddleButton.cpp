#include "ace.MiddleButton.h"

namespace ace{
	MiddleButton::MiddleButton(eMouseButtonState eMouseButtonState, double rotation)
	{
		m_eMouseButtonState = eMouseButtonState;
		m_rotation = rotation;
	}

	eMouseButtonState MiddleButton::GetButtonState() const
	{
		return m_eMouseButtonState;
	}

	const double MiddleButton::GetRotation() const
	{
		return m_rotation;
	}
};