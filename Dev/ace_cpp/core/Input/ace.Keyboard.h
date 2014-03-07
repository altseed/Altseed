﻿#pragma once

namespace ace{

	enum eKeys
	{
		ACE_KEY_UNKNOWN,
		ACE_KEY_SPACE,
		ACE_KEY_APOSTROPHE,
		ACE_KEY_COMMA,
		ACE_KEY_MINUS,
		ACE_KEY_PERIOD,
		ACE_KEY_SLASH,
		ACE_KEY_0,
		ACE_KEY_1,
		ACE_KEY_2,
		ACE_KEY_3,
		ACE_KEY_4,
		ACE_KEY_5,
		ACE_KEY_6,
		ACE_KEY_7,
		ACE_KEY_8,
		ACE_KEY_9,
		ACE_KEY_SEMICOLON,
		ACE_KEY_EQUAL,
		ACE_KEY_A,
		ACE_KEY_B,
		ACE_KEY_C,
		ACE_KEY_D,
		ACE_KEY_E,
		ACE_KEY_F,
		ACE_KEY_G,
		ACE_KEY_H,
		ACE_KEY_I,
		ACE_KEY_J,
		ACE_KEY_K,
		ACE_KEY_L,
		ACE_KEY_M,
		ACE_KEY_N,
		ACE_KEY_O,
		ACE_KEY_P,
		ACE_KEY_Q,
		ACE_KEY_R,
		ACE_KEY_S,
		ACE_KEY_T,
		ACE_KEY_U,
		ACE_KEY_V,
		ACE_KEY_W,
		ACE_KEY_X,
		ACE_KEY_Y,
		ACE_KEY_Z,
		ACE_KEY_LEFT_BRACKET,
		ACE_KEY_BACKSLASH,
		ACE_KEY_RIGHT_BRACKET,
		ACE_KEY_GRAVE_ACCENT,
		ACE_KEY_WORLD_1,
		ACE_KEY_WORLD_2,
		ACE_KEY_ESCAPE,
		ACE_KEY_ENTER,
		ACE_KEY_TAB,
		ACE_KEY_BACKSPACE,
		ACE_KEY_INSERT,
		ACE_KEY_DELETE,
		ACE_KEY_RIGHT,
		ACE_KEY_LEFT,
		ACE_KEY_DOWN,
		ACE_KEY_UP,
		ACE_KEY_PAGE_UP,
		ACE_KEY_PAGE_DOWN,
		ACE_KEY_HOME,
		ACE_KEY_END,
		ACE_KEY_CAPS_LOCK,
		ACE_KEY_SCROLL_LOCK,
		ACE_KEY_NUM_LOCK,
		ACE_KEY_PRINT_SCREEN,
		ACE_KEY_PAUSE,
		ACE_KEY_F1,
		ACE_KEY_F2,
		ACE_KEY_F3,
		ACE_KEY_F4,
		ACE_KEY_F5,
		ACE_KEY_F6,
		ACE_KEY_F7,
		ACE_KEY_F8,
		ACE_KEY_F9,
		ACE_KEY_F10,
		ACE_KEY_F11,
		ACE_KEY_F12,
		ACE_KEY_F13,
		ACE_KEY_F14,
		ACE_KEY_F15,
		ACE_KEY_F16,
		ACE_KEY_F17,
		ACE_KEY_F18,
		ACE_KEY_F19,
		ACE_KEY_F20,
		ACE_KEY_F21,
		ACE_KEY_F22,
		ACE_KEY_F23,
		ACE_KEY_F24,
		ACE_KEY_F25,
		ACE_KEY_KP_0,
		ACE_KEY_KP_1,
		ACE_KEY_KP_2,
		ACE_KEY_KP_3,
		ACE_KEY_KP_4,
		ACE_KEY_KP_5,
		ACE_KEY_KP_6,
		ACE_KEY_KP_7,
		ACE_KEY_KP_8,
		ACE_KEY_KP_9,
		ACE_KEY_KP_DECIMAL,
		ACE_KEY_KP_DIVIDE,
		ACE_KEY_KP_MULTIPLY,
		ACE_KEY_KP_SUBTRACT,
		ACE_KEY_KP_ADD,
		ACE_KEY_KP_ENTER,
		ACE_KEY_KP_EQUAL,
		ACE_KEY_LEFT_SHIFT,
		ACE_KEY_LEFT_CONTROL,
		ACE_KEY_LEFT_ALT,
		ACE_KEY_LEFT_SUPER,
		ACE_KEY_RIGHT_SHIFT,
		ACE_KEY_RIGHT_CONTROL,
		ACE_KEY_RIGHT_ALT,
		ACE_KEY_RIGHT_SUPER,
		ACE_KEY_MENU,
		ACE_KEY_LAST,
		MAX
	};

	enum eKeyboardButtonState
	{
		KEYBOARD_PUSH, KEYBOARD_PULL, KEYBOARD_HOLD, KEYBOARD_FREE
	};

	class Keyboard
	{
	public:
		virtual const eKeyboardButtonState GetKeyState(eKeys key) = 0;
	};

};