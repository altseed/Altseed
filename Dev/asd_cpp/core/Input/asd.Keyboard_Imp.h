#pragma once

#include "../Window/asd.Window_Imp.h"
#include "asd.Keyboard.h"

namespace asd{

	class Keyboard_Imp:public Keyboard
	{
	public:
		static Keyboard_Imp* Create(Window_Imp *window_Imp);
		void RefreshInputState();
		KeyState GetKeyState(Keys key) const override;
		Keyboard_Imp(Window_Imp *window_Imp);
		virtual ~Keyboard_Imp();
	private:
		static const int KEY_NUM = 121;
		static const int m_keyCode[128];
		bool m_currentHit[KEY_NUM];
		bool m_preHit[KEY_NUM];
		GLFWwindow* m_window;
	};

}