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
		preHitLeft = false;
		preHitMiddle = false;
		preHitRight = false;

		GLFWwindow *window = window_Imp->GetWindow();
		glfwSetScrollCallback(window, GetWheel);

		m_Window = window;
		wheelCalled = false;
	}

	void Mouse_Imp::RefreshInputState()
	{
		double mx, my;
		glfwGetCursorPos(m_Window, &mx, &my);
		m_Position = Vector2DF((float) mx, (float) my);

		int leftState = glfwGetMouseButton(m_Window, GLFW_MOUSE_BUTTON_LEFT);
		if (leftState == GLFW_PRESS){
			m_LeftButton = new SideButton((preHitLeft)?eMouseButtonState::MOUSE_HOLD:eMouseButtonState::MOUSE_PUSH,false);
			preHitLeft = true;
		}
		else{
			m_LeftButton = new SideButton((preHitLeft) ? eMouseButtonState::MOUSE_PULL : eMouseButtonState::MOUSE_FREE , false);
			preHitLeft = false;
		}

		int middleState = glfwGetMouseButton(m_Window, GLFW_MOUSE_BUTTON_MIDDLE);
		if (middleState == GLFW_PRESS){
			m_MiddleButton = new MiddleButton((preHitMiddle) ? eMouseButtonState::MOUSE_HOLD : eMouseButtonState::MOUSE_PUSH, (wheelCalled) ? yWheel : 0);
			preHitMiddle = true;
		}
		else{
			m_MiddleButton = new MiddleButton((preHitMiddle) ? eMouseButtonState::MOUSE_PULL : eMouseButtonState::MOUSE_FREE, (wheelCalled) ? yWheel : 0);
			preHitMiddle = false;
		}
		

		int rightState = glfwGetMouseButton(m_Window, GLFW_MOUSE_BUTTON_RIGHT);
		if (rightState == GLFW_PRESS){
			m_RightButton = new SideButton((preHitRight) ? eMouseButtonState::MOUSE_HOLD : eMouseButtonState::MOUSE_PUSH, false);
			preHitRight = true;
		}
		else{
			m_RightButton = new SideButton((preHitRight) ? eMouseButtonState::MOUSE_PULL : eMouseButtonState::MOUSE_FREE, false);
			preHitRight = false;
		}

		wheelCalled = false;
	}

	const SideButton* Mouse_Imp::GetLeftButton() const
	{
		return m_LeftButton;
	}

	const SideButton* Mouse_Imp::GetRightButton() const
	{
		return m_RightButton;
	}

	const MiddleButton* Mouse_Imp::GetMiddleButton() const
	{
		return m_MiddleButton;
	}

	const Vector2DF Mouse_Imp::GetPosition() const
	{
		return m_Position;
	}
}