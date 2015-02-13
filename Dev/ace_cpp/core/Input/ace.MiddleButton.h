#pragma once

#include "ace.MouseButtonState.h"

namespace ace{

	class MiddleButton
	{
	private:
		MouseButtonState m_mouseButtonState;
		double m_rotation;
	public:
		virtual const MouseButtonState GetButtonState() const;
		virtual const double GetRotation() const;
		MiddleButton(MouseButtonState mouseButtonState,double rotation);
		MiddleButton(){}
	};
};