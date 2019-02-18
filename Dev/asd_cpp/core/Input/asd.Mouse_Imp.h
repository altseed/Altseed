#pragma once

#include "../Window/asd.Window_Imp.h"
#include "asd.Mouse.h"
#include <array>

namespace asd
{
	class Mouse_Imp
		: public Mouse
	{
		Window_Imp*	window = nullptr;
		ap::Mouse*	mouse = nullptr;
		
	public:
		static Mouse_Imp* Create(Window_Imp* window);
		SideButton* GetLeftButton() const override;
		SideButton* GetRightButton() const override;
		MiddleButton* GetMiddleButton() const override;
		Vector2DF GetPosition() const override;
		void SetPosition(Vector2DF pos) override;
		void RefreshInputState();

		ButtonState GetButtonInputState(MouseButtons mouseButton) const override;
		float GetWheel() const override;

		ap::Mouse* GetNative() const { return mouse; }

		virtual ~Mouse_Imp();

	private:
		Mouse_Imp(Window_Imp* window);

		Vector2DF m_position;
		SideButton* m_leftButton;
		SideButton* m_rightButton;
		MiddleButton* m_middleButton;

		std::array<ButtonState, 8> buttonInputStates;
	};
};