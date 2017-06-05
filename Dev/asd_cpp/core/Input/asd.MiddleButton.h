#pragma once

#include "asd.MouseButtonState.h"

namespace asd{

	/**
	@brief マウスの中ボタンの状態を扱うクラス
	*/
	class MiddleButton
	{
		friend class Mouse;

	private:
		MouseButtonState m_mouseButtonState;
		double m_rotation;
	public:
		/**
		@brief ボタンの押し状態を返す
		*/
		virtual MouseButtonState GetButtonState() const;

		/**
		@brief ホイールの回転度合いを返す
		*/
		virtual float GetRotation() const;
		MiddleButton(MouseButtonState mouseButtonState, float rotation);
		MiddleButton(){}
	};
};