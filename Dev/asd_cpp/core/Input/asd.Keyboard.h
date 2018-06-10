#pragma once

namespace asd{
	/**
	@brief	Altseedで扱うキーボードのキー一覧
	*/
	enum class Keys : int32_t
	{
		Unknown,
		Space,			///< スペース
		Apostrophe,		///< ： (US配列では ` )
		Comma,			///< ，
		Minus,			///< －
		Period,			///< ．
		Slash,			///< ／
		Num0,			///< 0
		Num1,			///< 1
		Num2,			///< 2
		Num3,			///< 3
		Num4,			///< 4
		Num5,			///< 5
		Num6,			///< 6
		Num7,			///< 7
		Num8,			///< 8
		Num9,			///< 9
		Semicolon,		///< ；
		Equal,			///< ＾ (US配列では ＝ )
		A,				///< A
		B,				///< B
		C,				///< C
		D,				///< D
		E	,			///< E
		F,				///< F
		G,				///< G
		H,				///< H
		I,				///< I
		J,				///< J
		K,				///< K
		L,				///< L
		M,				///< M
		N,				///< N
		O,				///< O
		P,				///< P
		Q,				///< Q
		R,				///< R
		S,				///< S
		T,				///< T
		U,				///< U
		V,				///< V
		W,				///< W
		X,				///< X
		Y,				///< Y
		Z,				///< Z
		LeftBracket,	///< @ (US配列では ［ )
		Backslash,		///< ] (US配列では ＼ )
		RightBracket,	///< [ (US配列では ］ )
		GraveAccent,	///< 半角/全角
		World1,
		World2,
		Escape,			///< Esc
		Enter,			///< Enter
		Tab,			///< Tab 
		Backspace,		///< BackSpace
		Insert,			///< Insert 
		Delete,			///< Delete 
		Right,			///< →
		Left,			///< ←
		Down,			///< ↓
		Up,				///< ↑
		PageUp,			///< PageUp
		PageDown,		///< PageDown
		Home,			///< Home 
		End,			///< End
		CapsLock,		///< CapsLock
		ScrollLock,		///< ScrollLock
		NumLock,		///< NumLock
		PrintScreen,	///< PrintScreen
		Pause,			///< Pause
		F1,				///< F1
		F2,				///< F2
		F3,				///< F3
		F4,				///< F4
		F5,				///< F5
		F6,				///< F6
		F7,				///< F7
		F8,				///< F8
		F9,				///< F9
		F10,			///< F10
		F11,			///< F11
		F12,			///< F12
		F13,			///< F13
		F14,			///< F14
		F15,			///< F15
		F16,			///< F16
		F17,			///< F17
		F18,			///< F18
		F19,			///< F19
		F20,			///< F20
		F21,			///< F21
		F22,			///< F22
		F23,			///< F23
		F24,			///< F24
		F25,			///< F25
		Keypad0,		///< テンキーの0
		Keypad1,		///< テンキーの1
		Keypad2,		///< テンキーの2
		Keypad3,		///< テンキーの3
		Keypad4,		///< テンキーの4
		Keypad5,		///< テンキーの5
		Keypad6,		///< テンキーの6
		Keypad7,		///< テンキーの7
		Keypad8,		///< テンキーの8
		Keypad9,		///< テンキーの9
		KeypadDecimal,	///< テンキーの．
		KeypadDivide,	///< テンキーの／
		KeypadMultiply,	///< テンキーの＊
		KeypadSubstract,///< テンキーの－
		KeypadAdd,		///< テンキーの＋
		KeypadEnter,	///< テンキーのEnter
		KeypadEqual,	///< テンキーの＝
		LeftShift,		///< 左Shift
		LeftControl,	///< 左Ctrl
		LeftAlt,		///< 左Alt
		LeftWin,		///< 左Win
		RightShift,		///< 右Shift
		RightControl,	///< 右Ctrl
		RightAlt,		///< 右Alt
		RightWin,		///< 右Win
		Menu,			///< コンテキストメニュー
		Last,
		MAX
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
		virtual ButtonState GetKeyState(Keys key) const = 0;
	};

};