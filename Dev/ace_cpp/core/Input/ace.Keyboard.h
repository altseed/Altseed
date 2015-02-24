#pragma once

namespace ace{
	/**
	@brief	ACEで扱うキーボードのキー一覧
	*/
	enum class Keys : int32_t
	{
		Unknown,
		Space,
		Apostrophe,
		Comma,
		Minus,
		Period,
		Slash,
		Num0,
		Num1,
		Num2,
		Num3,
		Num4,
		Num5,
		Num6,
		Num7,
		Num8,
		Num9,
		Semicolon,
		Equal,
		A,
		B,
		C,
		D,
		E,
		F,
		G,
		H,
		I,
		J,
		K,
		L,
		M,
		N,
		O,
		P,
		Q,
		R,
		S,
		T,
		U,
		V,
		W,
		X,
		Y,
		Z,
		LeftBracket,
		Backslash,
		RightBracket,
		GraveAccent,
		World1,
		World2,
		Escape,
		Enter,
		Tab,
		Backspace,
		Insert,
		Delete,
		Right,
		Left,
		Down,
		Up,
		PageUp,
		PageDown,
		Home,
		End,
		CapsLock,
		ScrollLock,
		NumLock,
		PrintScreen,
		Pause,
		F1,
		F2,
		F3,
		F4,
		F5,
		F6,
		F7,
		F8,
		F9,
		F10,
		F11,
		F12,
		F13,
		F14,
		F15,
		F16,
		F17,
		F18,
		F19,
		F20,
		F21,
		F22,
		F23,
		F24,
		F25,
		Keypad0,
		Keypad1,
		Keypad2,
		Keypad3,
		Keypad4,
		Keypad5,
		Keypad6,
		Keypad7,
		Keypad8,
		Keypad9,
		KeypadDecimal,
		KeypadDivide,
		KeypadMultiply,
		KeypadSubstract,
		KeypadAdd,
		KeypadEnter,
		KeypadEqual,
		LeftShift,
		LeftControl,
		LeftAlt,
		LeftWin,
		RightShift,
		RightControl,
		RightAlt,
		RightWin,
		Menu,
		Last,
		MAX
	};

	/**
	@brief キーボードの押下状態を示す列挙型
	*/
	enum class KeyState : int32_t
	{
		/**
		@brief キーをこのフレームで押した状態
		*/
		Push,

		/**
		@brief キーをこのフレームで離した状態
		*/
		Release,

		/**
		@brief キーを押し続けている状態
		*/
		Hold,

		/**
		@brief キーを離し続けている状態
		*/
		Free
	};

	/**
	@brief キーボードからの入力を取得するクラス
	*/
	class Keyboard
	{
	public:
		/**
		@brief 特定のキーの押し状態をKeyState列挙型で返す
		@param 押し状態を調べたいキー
		@return 押し状態
		*/
		virtual const KeyState GetKeyState(Keys key) = 0;
	};

};