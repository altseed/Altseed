#pragma once

#include "asd.MouseButtonState.h"

namespace asd{

	/**
	@brief マウスの中ボタンの状態を扱うクラス
	*/
	class MiddleButton
	{
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
		virtual double GetRotation() const;
		MiddleButton(MouseButtonState mouseButtonState,double rotation);
		MiddleButton(){}
	};
};