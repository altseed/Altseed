#include "asd.Keyboard_Imp.h"

namespace asd
{
	Keyboard_Imp* Keyboard_Imp::Create(Window_Imp *window_Imp)
	{
		auto keyboard = new Keyboard_Imp(window_Imp);
		return keyboard;
	}

	Keyboard_Imp::Keyboard_Imp(Window_Imp* window)
	{
		this->window = window;
		keyboard = ap::Keyboard::Create(window->GetWindow());
		SafeAddRef(this->window);
	}

	Keyboard_Imp::~Keyboard_Imp()
	{
		SafeDelete(keyboard);
		SafeRelease(window);
	}

	void Keyboard_Imp::RefreshInputState()
	{
		assert(keyboard != nullptr);
		keyboard->RefreshInputState();
	}

	KeyState Keyboard_Imp::GetKeyState(Keys key) const
	{
		assert(keyboard != nullptr);
		return (KeyState)keyboard->GetKeyState((ap::Keys)key);
	}
};