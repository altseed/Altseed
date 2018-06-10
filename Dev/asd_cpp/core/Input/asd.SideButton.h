#pragma once

#include "asd.MouseButtonState.h"

#include "../asd.Core.Base.h"

namespace asd{

	/**
	@brief マウスの左右ボタンの状態を扱うクラス
	*/
	class SideButton
	{
	private:
		ButtonState m_mouseButtonState;
	public:
		/**
		@brief ボタンの押し状態を返す
		@return ボタンの押し状態
		*/
		virtual ButtonState GetButtonState() const;

		SideButton(){}
		SideButton(ButtonState mouseButtonState,bool doubleClicked);
	};
};
