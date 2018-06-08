#pragma once

#include "../Window/asd.Window_Imp.h"
#include "asd.Keyboard.h"

namespace asd{

	class Keyboard_Imp
		: public Keyboard
	{
	private:
		Window_Imp*	window = nullptr;
		ap::Keyboard*	keyboard = nullptr;

	public:
		static Keyboard_Imp* Create(Window_Imp* window);
		void RefreshInputState();
		ButtonState GetKeyState(Keys key) const override;
		Keyboard_Imp(Window_Imp* window);
		virtual ~Keyboard_Imp();
	};

}