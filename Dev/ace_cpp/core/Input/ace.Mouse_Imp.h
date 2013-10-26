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
		Vector2DF m_Position;
		GLFWwindow* m_Window;
		SideButton* m_LeftButton;
		SideButton* m_RightButton;
		MiddleButton* m_MiddleButton;
		bool preHitLeft;
		bool preHitMiddle;
		bool preHitRight;
		double wheel;
	};
};