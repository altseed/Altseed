#pragma once

namespace ace{

	/**
	@brief	ジョイスティックのボタンの押し状態を示す列挙型
	*/
	enum class JoystickButtonState : int32_t
	{
		/**
		@brief	ボタンをこのフレームで押した状態
		*/
		Push,

		/**
		@brief	ボタンをこのフレームで離した状態
		*/
		Release,

		/**
		@brief	ボタンを離し続けている状態
		*/
		Free,

		/**
		@brief	ボタンを押し続けている状態
		*/
		Hold
	};

	/**
	@brief	指定したコンポーネントをこのインスタンスに追加する。
	*/
	class Joystick
	{
	public:
		Joystick(){}
		virtual ~Joystick(){}

		/**
		@brief	ジョイスティックの名前を返す
		*/
		virtual const ace::achar* GetJoystickName() = 0;

		/**
		@brief	ジョイスティックのボタンの合計数を返す
		*/
		virtual int GetButtonsCount() = 0;

		/**
		@brief	ジョイスティックの軸の合計数を返す
		*/
		virtual int GetAxesCount() = 0;

		/**
		@brief	指定したボタンの押し状態を返す
		@param	at	何番目のボタンの押し状態を取得するか
		@return 押し状態
		*/
		virtual JoystickButtonState GetButtonState(int at) = 0;

		/**
		@brief	指定した軸の倒し具合を返す
		@param	at	何番目の軸配列の倒し具合を取得するか
		@return 倒し具合
		*/
		virtual float GetAxisState(int at) = 0;
	};
};
