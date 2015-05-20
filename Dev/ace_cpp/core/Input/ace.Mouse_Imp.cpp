#include "ace.Mouse.h"
#include "ace.Mouse_Imp.h"

double yWheel;
bool wheelCalled;
void GetWheel(GLFWwindow* wHandle,double x,double y){
	yWheel = y;
	wheelCalled = true;
}

namespace ace{

	Mouse_Imp* Mouse_Imp::Create(Window_Imp* window_Imp)
	{
		return new Mouse_Imp(window_Imp);
	}

	Mouse_Imp::Mouse_Imp(Window_Imp* window_Imp)
	{
		m_preHitLeft = false;
		m_preHitMiddle = false;
		m_preHitRight = false;

		m_leftButton = NULL;
		m_rightButton = NULL;
		m_middleButton = NULL;

		GLFWwindow *window = window_Imp->GetWindow();
		glfwSetScrollCallback(window, GetWheel);

		m_window = window;
		wheelCalled = false;
	}

	void Mouse_Imp::RefreshInputState()
	{
		double mx, my;
		glfwGetCursorPos(m_window, &mx, &my);
		m_position = Vector2DF((float) mx, (float) my);

		SafeDelete(m_leftButton);
		SafeDelete(m_rightButton);
		SafeDelete(m_middleButton);

		int leftState = glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_LEFT);
		if (leftState == GLFW_PRESS){
			m_leftButton = new SideButton((m_preHitLeft)?MouseButtonState::Hold:MouseButtonState::Push,false);
			m_preHitLeft = true;
		}
		else{
			m_leftButton = new SideButton((m_preHitLeft) ? MouseButtonState::Release : MouseButtonState::Free , false);
			m_preHitLeft = false;
		}

		int middleState = glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_MIDDLE);
		if (middleState == GLFW_PRESS){
			m_middleButton = new MiddleButton((m_preHitMiddle) ? MouseButtonState::Hold : MouseButtonState::Push, (wheelCalled) ? yWheel : 0);
			m_preHitMiddle = true;
		}
		else{
			m_middleButton = new MiddleButton((m_preHitMiddle) ? MouseButtonState::Release : MouseButtonState::Free, (wheelCalled) ? yWheel : 0);
			m_preHitMiddle = false;
		}
		

		int rightState = glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_RIGHT);
		if (rightState == GLFW_PRESS){
			m_rightButton = new SideButton((m_preHitRight) ? MouseButtonState::Hold : MouseButtonState::Push, false);
			m_preHitRight = true;
		}
		else{
			m_rightButton = new SideButton((m_preHitRight) ? MouseButtonState::Release : MouseButtonState::Free, false);
			m_preHitRight = false;
		}

		wheelCalled = false;
	}

	SideButton* Mouse_Imp::GetLeftButton() const
	{
		return m_leftButton;
	}

	SideButton* Mouse_Imp::GetRightButton() const
	{
		return m_rightButton;
	}

	MiddleButton* Mouse_Imp::GetMiddleButton() const
	{
		return m_middleButton;
	}

	Vector2DF Mouse_Imp::GetPosition() const
	{
		return m_position;
	}
}