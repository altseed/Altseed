#include "ace.Keyboard_Imp.h"

namespace ace{

	const int Keyboard_Imp::m_keyCode[] =
	{
		GLFW_KEY_UNKNOWN,
		GLFW_KEY_SPACE,
		GLFW_KEY_APOSTROPHE,
		GLFW_KEY_COMMA,
		GLFW_KEY_MINUS,
		GLFW_KEY_PERIOD,
		GLFW_KEY_SLASH,
		GLFW_KEY_0,
		GLFW_KEY_1,
		GLFW_KEY_2,
		GLFW_KEY_3,
		GLFW_KEY_4,
		GLFW_KEY_5,
		GLFW_KEY_6,
		GLFW_KEY_7,
		GLFW_KEY_8,
		GLFW_KEY_9,
		GLFW_KEY_SEMICOLON,
		GLFW_KEY_EQUAL,
		GLFW_KEY_A,
		GLFW_KEY_B,
		GLFW_KEY_C,
		GLFW_KEY_D,
		GLFW_KEY_E,
		GLFW_KEY_F,
		GLFW_KEY_G,
		GLFW_KEY_H,
		GLFW_KEY_I,
		GLFW_KEY_J,
		GLFW_KEY_K,
		GLFW_KEY_L,
		GLFW_KEY_M,
		GLFW_KEY_N,
		GLFW_KEY_O,
		GLFW_KEY_P,
		GLFW_KEY_Q,
		GLFW_KEY_R,
		GLFW_KEY_S,
		GLFW_KEY_T,
		GLFW_KEY_U,
		GLFW_KEY_V,
		GLFW_KEY_W,
		GLFW_KEY_X,
		GLFW_KEY_Y,
		GLFW_KEY_Z,
		GLFW_KEY_LEFT_BRACKET,
		GLFW_KEY_BACKSLASH,
		GLFW_KEY_RIGHT_BRACKET,
		GLFW_KEY_GRAVE_ACCENT,
		GLFW_KEY_WORLD_1,
		GLFW_KEY_WORLD_2,
		GLFW_KEY_ESCAPE,
		GLFW_KEY_ENTER,
		GLFW_KEY_TAB,
		GLFW_KEY_BACKSPACE,
		GLFW_KEY_INSERT,
		GLFW_KEY_DELETE,
		GLFW_KEY_RIGHT,
		GLFW_KEY_LEFT,
		GLFW_KEY_DOWN,
		GLFW_KEY_UP,
		GLFW_KEY_PAGE_UP,
		GLFW_KEY_PAGE_DOWN,
		GLFW_KEY_HOME,
		GLFW_KEY_END,
		GLFW_KEY_CAPS_LOCK,
		GLFW_KEY_SCROLL_LOCK,
		GLFW_KEY_NUM_LOCK,
		GLFW_KEY_PRINT_SCREEN,
		GLFW_KEY_PAUSE,
		GLFW_KEY_F1,
		GLFW_KEY_F2,
		GLFW_KEY_F3,
		GLFW_KEY_F4,
		GLFW_KEY_F5,
		GLFW_KEY_F6,
		GLFW_KEY_F7,
		GLFW_KEY_F8,
		GLFW_KEY_F9,
		GLFW_KEY_F10,
		GLFW_KEY_F11,
		GLFW_KEY_F12,
		GLFW_KEY_F13,
		GLFW_KEY_F14,
		GLFW_KEY_F15,
		GLFW_KEY_F16,
		GLFW_KEY_F17,
		GLFW_KEY_F18,
		GLFW_KEY_F19,
		GLFW_KEY_F20,
		GLFW_KEY_F21,
		GLFW_KEY_F22,
		GLFW_KEY_F23,
		GLFW_KEY_F24,
		GLFW_KEY_F25,
		GLFW_KEY_KP_0,
		GLFW_KEY_KP_1,
		GLFW_KEY_KP_2,
		GLFW_KEY_KP_3,
		GLFW_KEY_KP_4,
		GLFW_KEY_KP_5,
		GLFW_KEY_KP_6,
		GLFW_KEY_KP_7,
		GLFW_KEY_KP_8,
		GLFW_KEY_KP_9,
		GLFW_KEY_KP_DECIMAL,
		GLFW_KEY_KP_DIVIDE,
		GLFW_KEY_KP_MULTIPLY,
		GLFW_KEY_KP_SUBTRACT,
		GLFW_KEY_KP_ADD,
		GLFW_KEY_KP_ENTER,
		GLFW_KEY_KP_EQUAL,
		GLFW_KEY_LEFT_SHIFT,
		GLFW_KEY_LEFT_CONTROL,
		GLFW_KEY_LEFT_ALT,
		GLFW_KEY_LEFT_SUPER,
		GLFW_KEY_RIGHT_SHIFT,
		GLFW_KEY_RIGHT_CONTROL,
		GLFW_KEY_RIGHT_ALT,
		GLFW_KEY_RIGHT_SUPER,
		GLFW_KEY_MENU,
		GLFW_KEY_MENU
	};

	const int Keyboard_Imp::m_KeyNum = 121;

	Keyboard_Imp::Keyboard_Imp(Window_Imp *window_Imp)
	{
		GLFWwindow *window = window_Imp->GetWindow();
		m_window = window;
		for (int i = 0; i < m_KeyNum; i++){
			m_currentHit[i] = GLFW_RELEASE;
			m_preHit[i] = GLFW_RELEASE;
		}
	}
		
	Keyboard_Imp* Keyboard_Imp::Create(Window_Imp *window_Imp)
	{
		Keyboard_Imp* keyboard = new Keyboard_Imp(window_Imp);
		return keyboard;
	}

	Keyboard_Imp::~Keyboard_Imp()
	{

	}

	void Keyboard_Imp::RefreshInputState()
	{
		for (int i = 0; i < m_KeyNum; i++){
			m_preHit[i] = m_currentHit[i];
			m_currentHit[i] = (bool)glfwGetKey(m_window, m_keyCode[i]);
		}
	}

	const KeyState Keyboard_Imp::GetKeyState(Keys key)
	{
		int index = (int)key;
		if (m_currentHit[index] && m_preHit[index]) return KeyState::Hold;
		else if (!m_currentHit[index] && m_preHit[index]) return KeyState::Release;
		else if (m_currentHit[index] && !m_preHit[index]) return KeyState::Push;
		else return KeyState::Free;
	}
};