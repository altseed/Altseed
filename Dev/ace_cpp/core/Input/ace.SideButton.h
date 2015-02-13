#pragma once

#include "ace.MouseButtonState.h"

namespace ace{

	class SideButton
	{
	private:
		MouseButtonState m_mouseButtonState;
	public:
		virtual const MouseButtonState GetButtonState() const;
		SideButton(){}
		SideButton(MouseButtonState mouseButtonState,bool doubleClicked);
	};
};
