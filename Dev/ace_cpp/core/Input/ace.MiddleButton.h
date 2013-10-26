#pragma once

#include "ace.IMouseWheelableButtonState.h"

namespace ace{

	class MiddleButton : public IMouseWheelableButtonState
	{
	private:
		eMouseButtonState m_eMouseButtonState;
		double m_rotation;
	public:
		eMouseButtonState GetButtonState() const;
		const double GetRotation() const;
		MiddleButton(eMouseButtonState eMouseButtonState,double rotation);
		MiddleButton(){}
	};
};