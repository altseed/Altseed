#include "ace.SideButton.h"

namespace ace{
	SideButton::SideButton(eMouseButtonState eMouseButtonState,bool doubleClicked)
	{
		m_eMouseButtonState = eMouseButtonState;
	}

	eMouseButtonState SideButton::GetButtonState() const
	{
		return m_eMouseButtonState;
	}
};