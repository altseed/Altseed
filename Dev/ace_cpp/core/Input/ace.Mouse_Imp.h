#pragma once

#include "../Window/ace.Window_Imp.h"
#include "ace.Mouse.h"

namespace ace{
	class Mouse_Imp:public Mouse
	{
	public:
		static Mouse_Imp* Create(Window_Imp* window_Imp);
		const SideButton* GetLeftButton() const;
		const SideButton* GetRightButton() const;
		const MiddleButton* GetMiddleButton() const;
		const Vector2DF GetPosition() const;
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