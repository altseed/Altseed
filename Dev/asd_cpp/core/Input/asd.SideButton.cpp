#include "asd.SideButton.h"

namespace asd{
	SideButton::SideButton(ButtonState mouseButtonState,bool doubleClicked)
	{
		m_mouseButtonState = mouseButtonState;
	}

	ButtonState SideButton::GetButtonState() const
	{
		return m_mouseButtonState;
	}
};