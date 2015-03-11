#pragma once

#include "../Window/ace.Window_Imp.h"
#include "ace.Mouse.h"

namespace ace{
	class Mouse_Imp:public Mouse
	{
	public:
		static Mouse_Imp* Create(Window_Imp* window_Imp);
		SideButton* GetLeftButton() const override;
		SideButton* GetRightButton() const override;
		MiddleButton* GetMiddleButton() const override;
		Vector2DF GetPosition() const override;
		void RefreshInputState();
	private:
		Mouse_Imp(Window_Imp* window_Imp);
		Vector2DF m_position;
		GLFWwindow* m_window;
		SideButton* m_leftButton;
		SideButton* m_rightButton;
		MiddleButton* m_middleButton;
		bool m_preHitLeft;
		bool m_preHitMiddle;
		bool m_preHitRight;
		double m_wheel;
	};
};