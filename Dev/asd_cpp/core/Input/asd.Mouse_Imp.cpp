#include "asd.Mouse.h"
#include "asd.Mouse_Imp.h"

static double yWheel = 0;
static bool wheelCalled = false;
static void GetWheelInternal(GLFWwindow* wHandle,double x,double y){
	yWheel = y;
	wheelCalled = true;
}

namespace asd{

	Mouse_Imp* Mouse_Imp::Create(Window_Imp* window_Imp)
	{
		return new Mouse_Imp(window_Imp);
	}

	Mouse_Imp::Mouse_Imp(Window_Imp* window_Imp)
	{
		preHits.fill(false);
		buttonInputStates.fill(asd::MouseButtonState::Free);

		m_leftButton = NULL;
		m_rightButton = NULL;
		m_middleButton = NULL;

		GLFWwindow *window = window_Imp->GetWindow();
		glfwSetScrollCallback(window, GetWheelInternal);

		m_window = window;
	}

	void Mouse_Imp::RefreshInputState()
	{
		double mx, my;
		glfwGetCursorPos(m_window, &mx, &my);
		m_position = Vector2DF((float) mx, (float) my);

		SafeDelete(m_leftButton);
		SafeDelete(m_rightButton);
		SafeDelete(m_middleButton);

		for (int buttonIndex = 0; buttonIndex < 8; ++buttonIndex)
		{
			int state = glfwGetMouseButton(m_window, buttonIndex);
			if (state == GLFW_PRESS)
			{
				buttonInputStates[buttonIndex] = preHits[buttonIndex] ? MouseButtonState::Hold : MouseButtonState::Push;
				preHits[buttonIndex] = true;
			}
			else
			{
				buttonInputStates[buttonIndex] = preHits[buttonIndex] ? MouseButtonState::Release : MouseButtonState::Free;
				preHits[buttonIndex] = false;
			}
		}

		yWheel = (wheelCalled) ? yWheel : 0;

		m_leftButton = new SideButton(buttonInputStates[0], false);
		m_rightButton = new SideButton(buttonInputStates[1], false);
		m_middleButton = new MiddleButton(buttonInputStates[2], yWheel);

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

	void Mouse_Imp::SetPosition(Vector2DF pos)
	{
		double x = pos.X;
		double y = pos.Y;
		m_position = pos;

		glfwSetCursorPos(m_window, x, y);
	}

	MouseButtonState Mouse_Imp::GetButtonInputState(MouseButtons mouseButton) const
	{
		return buttonInputStates[static_cast<int>(mouseButton)];
	}

	float Mouse_Imp::GetWheel() const
	{
		return yWheel;
	}
}