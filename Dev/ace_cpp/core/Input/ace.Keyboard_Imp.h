#pragma once

#include "../Window/ace.Window_Imp.h"
#include "ace.Keyboard.h"

namespace ace{

	class Keyboard_Imp:public Keyboard
	{
	public:
		static Keyboard_Imp* Create(Window_Imp *window_Imp);
		void RefreshInputState();
		const KeyboardButtonState GetKeyState(Keys key);
		Keyboard_Imp(Window_Imp *window_Imp);
		virtual ~Keyboard_Imp();
	private:
		static const int m_keyCode[128];
		static const int m_KeyNum;
		bool m_currentHit[121];
		bool m_preHit[121];
		GLFWwindow* m_window;
	};

}