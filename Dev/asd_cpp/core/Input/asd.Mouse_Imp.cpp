#include "asd.Mouse.h"
#include "asd.Mouse_Imp.h"

namespace asd
{

	Mouse_Imp* Mouse_Imp::Create(Window_Imp* window)
	{
		return new Mouse_Imp(window);
	}

	Mouse_Imp::Mouse_Imp(Window_Imp* window)
	{
		buttonInputStates.fill(ButtonState::Free);

		m_leftButton = new SideButton(buttonInputStates[0], false);
		m_rightButton = new SideButton(buttonInputStates[1], false);
		m_middleButton = new MiddleButton(buttonInputStates[2], 0);

		this->mouse = ap::Mouse::Create(window->GetWindow());
		this->window = window;
		SafeAddRef(this->window);
	}

	Mouse_Imp::~Mouse_Imp()
	{
		SafeDelete(m_leftButton);
		SafeDelete(m_rightButton);
		SafeDelete(m_middleButton);

		SafeDelete(mouse);
		SafeRelease(this->window);
	}

	void Mouse_Imp::RefreshInputState()
	{
		assert(mouse != nullptr);
		mouse->RefreshInputState();

		float mx, my;
		mouse->GetPosition(mx, my);
		m_position = Vector2DF((float) mx, (float) my);

		for (int buttonIndex = 0; buttonIndex < 8; ++buttonIndex)
		{
			buttonInputStates[buttonIndex] = (ButtonState)mouse->GetMouseButtonState((ap::MouseButtons)buttonIndex);
		}

		SafeDelete(m_leftButton);
		SafeDelete(m_rightButton);
		SafeDelete(m_middleButton);
		m_leftButton = new SideButton(buttonInputStates[0], false);
		m_rightButton = new SideButton(buttonInputStates[1], false);
		m_middleButton = new MiddleButton(buttonInputStates[2], GetWheel());
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

		mouse->SetPosition(x, y);
	}

	ButtonState Mouse_Imp::GetButtonInputState(MouseButtons mouseButton) const
	{
		return buttonInputStates[static_cast<int>(mouseButton)];
	}

	float Mouse_Imp::GetWheel() const
	{
		return mouse->GetWheel();
	}
}