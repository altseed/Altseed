#pragma once

#include "asd.MouseButtonState.h"

#include "../asd.Core.Base.h"

namespace asd{

	/**
	@brief マウスの中ボタンの状態を扱うクラス
	*/
	class MiddleButton
	{
		friend class Mouse;

	private:
		ButtonState m_mouseButtonState;
		double m_rotation;
	public:
		/**
		@brief ボタンの押し状態を返す
		*/
		virtual ButtonState GetButtonState() const;

		/**
		@brief ホイールの回転度合いを返す
		*/
		virtual float GetRotation() const;
		MiddleButton(ButtonState mouseButtonState, float rotation);
		MiddleButton(){}
	};
};