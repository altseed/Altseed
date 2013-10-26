#pragma once

#include "../Window/ace.Window_Imp.h"
#include "ace.Keyboard.h"

namespace ace{

	class Keyboard_Imp:public Keyboard
	{
	public:
		static Keyboard_Imp* Create(Window_Imp *window_Imp);
		void RefreshInputState();
		const eKeyboardButtonState GetKeyState(eKeys key);
		Keyboard_Imp(Window_Imp *window_Imp);
		virtual ~Keyboard_Imp();
	private:
		static const int keyCode[128];
		static const int KeyNum;
		bool currentHit[121];
		bool preHit[121];
		GLFWwindow* m_window;
	};

}