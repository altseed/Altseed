#pragma once

#include "ace.IMouseWheelableButtonState.h"

namespace ace{

	class MiddleButton : public IMouseWheelableButtonState
	{
	private:
		MouseButtonState m_mouseButtonState;
		double m_rotation;
	public:
		const MouseButtonState GetButtonState() const;
		const double GetRotation() const;
		MiddleButton(MouseButtonState mouseButtonState,double rotation);
		MiddleButton(){}
	};
};