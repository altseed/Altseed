#pragma once

#include "ace.MouseButtonState.h"

namespace ace{

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
		virtual const MouseButtonState GetButtonState() const;

		/**
		@brief ホイールの回転度合いを返す
		*/
		virtual const double GetRotation() const;
		MiddleButton(MouseButtonState mouseButtonState,double rotation);
		MiddleButton(){}
	};
};