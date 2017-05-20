#pragma once

namespace asd{

	/**
		@brief	ジョイステイックの種類
	*/
	enum class JoystickType : int32_t
	{
		Other,
		PS4,
		XBOX360,
	};

	/**
		@brief	ジョイステイックのボタンの種類
	*/
	enum class JoystickButtonType : int32_t
	{
		Start,
		Select,

		LeftUp,
		LeftDown,
		LeftLeft,
		LeftRight,

		RightUp,	///< PSx Triangle
		RightRight,	///< PSx Circle
		RightLeft,	///< PSx Square
		RightDown,	///< PSx Cross

		L1,
		R1,
		L2,
		R2,
		L3,
		R3,

		LeftStart,	///< XBOX360 Start, PS4 Options
		RightStart,	///< XBOX360 Select, PS4 TouchPad

		Max,
	};

	/**
		@brief	ジョイステイックの軸の種類
	*/
	enum class JoystickAxisType : int32_t
	{
		LeftH,
		LeftV,
		RightH,
		RightV,
		L2,
		R2,

		Max,
	};

	/**
	@brief	ジョイスティックのボタンの押し状態を示す列挙型
	*/
	enum class JoystickButtonState : int32_t
	{
		/**
		@brief ボタンをこのフレームで押した状態
		*/
		Push,

		/**
		@brief ボタンをこのフレームで離した状態
		*/
		Release,

		/**
		@brief ボタンを押し続けている状態
		*/
		Hold,

		/**
		@brief ボタンを離し続けている状態
		*/
		Free
	};

	/**
	@brief	ジョイステイックのクラス
	*/
	class Joystick
	{
	public:
		Joystick(){}
		virtual ~Joystick(){}

		/**
		@brief	ジョイスティックの名称を取得する。
		@return	名称
		*/
		virtual const asd::achar* GetJoystickName() = 0;

		/**
			@brief	ジョイスティックの種類を取得する。
		*/
		virtual JoystickType GetJoystickType() const = 0;

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
			@brief	特定のジョイステイックの場合(PS4)に指定したボタンの押し状態を取得する。
			@param	type	ボタンの種類
			@return 押し状態
		*/
		virtual JoystickButtonState GetButtonStateAt(JoystickButtonType type) = 0;

		/**
		@brief	指定した軸の倒し具合を返す
		@param	at	何番目の軸配列の倒し具合を取得するか
		@return 倒し具合
		*/
		virtual float GetAxisState(int at) = 0;

		/**
		@brief	特定のジョイステイックの場合(PS4)に指定した指定した軸の倒し具合を取得する。
		@param	type	軸の種類
		@return 倒し具合
		*/
		virtual float GetAxisStateAt(JoystickAxisType type) = 0;
	};
};
