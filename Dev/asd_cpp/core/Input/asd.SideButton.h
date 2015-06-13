#pragma once

#include "asd.MouseButtonState.h"

namespace asd{

	/**
	@brief マウスの左右ボタンの状態を扱うクラス
	*/
	class SideButton
	{
	private:
		MouseButtonState m_mouseButtonState;
	public:
		/**
		@brief ボタンの押し状態を返す
		@return ボタンの押し状態
		*/
		virtual MouseButtonState GetButtonState() const;

		SideButton(){}
		SideButton(MouseButtonState mouseButtonState,bool doubleClicked);
	};
};
