#pragma once

#include "ace.IMouseButtonState.h"

namespace ace{

	class SideButton:public IMouseButtonState
	{
	private:
		MouseButtonState m_mouseButtonState;
	public:
		const MouseButtonState GetButtonState() const;
		SideButton(){}
		SideButton(MouseButtonState mouseButtonState,bool doubleClicked);
	};
};