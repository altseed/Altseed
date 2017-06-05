#pragma once

#include <memory>
#include "asd.MiddleButton.h"
#include "asd.SideButton.h"
#include "Math/asd.Vector2DF.h"

namespace asd{


	/**
	@brief マウスのボタン一覧を示す列挙型
	*/
	enum class MouseButtons : int32_t
	{
		/**
		@brief 左ボタン
		*/
		ButtonLeft = 0,

		/**
		@brief 右ボタン
		*/
		ButtonRight = 1,

		/**
		@brief 中央ボタン
		*/
		ButtonMiddle = 2,

		/**
		@brief サブボタン1
		*/
		SubButton1 = 3,

		/**
		@brief サブボタン2
		*/
		SubButton2 = 4,

		/**
		@brief サブボタン3
		*/
		SubButton3 = 5,

		/**
		@brief サブボタン4
		*/
		SubButton4 = 6,

		/**
		@brief サブボタン5
		*/
		SubButton5 = 7,
	};
	
	/**
	@brief マウスからの入力を取得するクラス
	*/
	class Mouse
	{
	public:
		/**
		@brief 非推奨である。(GetButtonInputStateを代わりに使用する)
		@note
		*/
		virtual SideButton* GetLeftButton() const = 0;


		/**
		@brief 非推奨である。(GetButtonInputStateを代わりに使用する)
		@note
		*/
		virtual SideButton* GetRightButton() const = 0;


		/**
		@brief 非推奨である。(GetButtonInputStateを代わりに使用する)
		*/
		virtual MiddleButton* GetMiddleButton() const = 0;


		/**
		@brief マウスポインタの位置を取得する。
		*/
		virtual Vector2DF GetPosition() const = 0;

		/**
			@brief マウスポインタの位置を設定する。
		*/
		virtual void SetPosition(Vector2DF pos) = 0;

		/**
		@brief 指定したボタンの入力状態を取得を取得する。
		*/
		virtual MouseButtonState GetButtonInputState(MouseButtons mouseButton) const = 0;


		/**
		@brief ホイールの回転度合いを取得する。
		*/
		virtual float GetWheel() const = 0;

	};
};