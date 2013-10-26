#pragma once

#include "ace.IMouseButtonState.h"

namespace ace{

	class SideButton:public IMouseButtonState
	{
	private:
		eMouseButtonState m_eMouseButtonState;
	public:
		eMouseButtonState GetButtonState() const;
		SideButton(){}
		SideButton(eMouseButtonState eMouseButtonState,bool doubleClicked);
	};
};