#pragma once

#include "ace.MouseButtonState.h"

namespace ace{

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
		virtual const MouseButtonState GetButtonState() const;

		SideButton(){}
		SideButton(MouseButtonState mouseButtonState,bool doubleClicked);
	};
};
