#pragma once

#include "../Window/asd.Window_Imp.h"
#include "asd.Mouse.h"
#include <array>

namespace asd{
	class Mouse_Imp:public Mouse
	{
	public:
		static Mouse_Imp* Create(Window_Imp* window_Imp);
		SideButton* GetLeftButton() const override;
		SideButton* GetRightButton() const override;
		MiddleButton* GetMiddleButton() const override;
		Vector2DF GetPosition() const override;
		void RefreshInputState();

		MouseButtonState GetButtonInputState(MouseButtons mouseButton) const override;
		float GetWheel() const override;
	private:
		Mouse_Imp(Window_Imp* window_Imp);
		Vector2DF m_position;
		GLFWwindow* m_window;
		SideButton* m_leftButton;
		SideButton* m_rightButton;
		MiddleButton* m_middleButton;

		std::array<bool, 8> preHits;
		std::array<MouseButtonState, 8> buttonInputStates;
		double m_wheel;
	};
};