using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
    /// <summary>
    /// キーボードの押下状態を示す列挙型
    /// </summary>
    public enum KeyState : int
    {
        /// <summary>
        ///Pull-キーを離した瞬間のみ返す
        /// </summary>
        Pull = ace.swig.eKeyboardButtonState.KEYBOARD_PULL,
        /// <summary>
        /// Push-キーを押した瞬間のみ返す
        /// </summary>
        Push = ace.swig.eKeyboardButtonState.KEYBOARD_PUSH,
        /// <summary>
        /// Free-キーを押していない時に返す
        /// </summary>
        Free = ace.swig.eKeyboardButtonState.KEYBOARD_FREE,
        /// <summary>
        /// Hold-キーを押している時に返す
        /// </summary>
        Hold = ace.swig.eKeyboardButtonState.KEYBOARD_HOLD
    };

    /// <summary>
    /// ACEで扱うキーボードのキー一覧
    /// </summary>
    public enum Keys : int
    {
        Unknown = ace.swig.eKeys.ACE_KEY_UNKNOWN,
        Space = ace.swig.eKeys.ACE_KEY_SPACE,
        Apostrophe = ace.swig.eKeys.ACE_KEY_APOSTROPHE,
        Comma = ace.swig.eKeys.ACE_KEY_COMMA,
        Minus = ace.swig.eKeys.ACE_KEY_MINUS,
        Period = ace.swig.eKeys.ACE_KEY_PERIOD,
        Slash = ace.swig.eKeys.ACE_KEY_SLASH,
        Number0 = ace.swig.eKeys.ACE_KEY_0,
        Number1 = ace.swig.eKeys.ACE_KEY_1,
        Number2 = ace.swig.eKeys.ACE_KEY_2,
        Number3 = ace.swig.eKeys.ACE_KEY_3,
        Number4 = ace.swig.eKeys.ACE_KEY_4,
        Number5 = ace.swig.eKeys.ACE_KEY_5,
        Number6 = ace.swig.eKeys.ACE_KEY_6,
        Number7 = ace.swig.eKeys.ACE_KEY_7,
        Number8 = ace.swig.eKeys.ACE_KEY_8,
        Number9 = ace.swig.eKeys.ACE_KEY_9,
        Semicolon = ace.swig.eKeys.ACE_KEY_SEMICOLON,
        Equal = ace.swig.eKeys.ACE_KEY_EQUAL,
        A = ace.swig.eKeys.ACE_KEY_A,
        B = ace.swig.eKeys.ACE_KEY_B,
        C = ace.swig.eKeys.ACE_KEY_C,
        D = ace.swig.eKeys.ACE_KEY_D,
        E = ace.swig.eKeys.ACE_KEY_E,
        F = ace.swig.eKeys.ACE_KEY_F,
        G = ace.swig.eKeys.ACE_KEY_G,
        H = ace.swig.eKeys.ACE_KEY_H,
        I = ace.swig.eKeys.ACE_KEY_I,
        J = ace.swig.eKeys.ACE_KEY_J,
        K = ace.swig.eKeys.ACE_KEY_K,
        L = ace.swig.eKeys.ACE_KEY_L,
        M = ace.swig.eKeys.ACE_KEY_M,
        N = ace.swig.eKeys.ACE_KEY_N,
        O = ace.swig.eKeys.ACE_KEY_O,
        P = ace.swig.eKeys.ACE_KEY_P,
        Q = ace.swig.eKeys.ACE_KEY_Q,
        R = ace.swig.eKeys.ACE_KEY_R,
        S = ace.swig.eKeys.ACE_KEY_S,
        T = ace.swig.eKeys.ACE_KEY_T,
        U = ace.swig.eKeys.ACE_KEY_U,
        V = ace.swig.eKeys.ACE_KEY_V,
        W = ace.swig.eKeys.ACE_KEY_W,
        X = ace.swig.eKeys.ACE_KEY_X,
        Y = ace.swig.eKeys.ACE_KEY_Y,
        Z = ace.swig.eKeys.ACE_KEY_Z,
        LeftBracket = ace.swig.eKeys.ACE_KEY_LEFT_BRACKET,
        BackSlash = ace.swig.eKeys.ACE_KEY_BACKSLASH,
        RightBracket = ace.swig.eKeys.ACE_KEY_RIGHT_BRACKET,
        GraveAccent = ace.swig.eKeys.ACE_KEY_GRAVE_ACCENT,
        World1 = ace.swig.eKeys.ACE_KEY_WORLD_1,
        World2 = ace.swig.eKeys.ACE_KEY_WORLD_2,
        Escape = ace.swig.eKeys.ACE_KEY_ESCAPE,
        Enter = ace.swig.eKeys.ACE_KEY_ENTER,
        Tab = ace.swig.eKeys.ACE_KEY_TAB,
        BackSpace = ace.swig.eKeys.ACE_KEY_BACKSPACE,
        Insert = ace.swig.eKeys.ACE_KEY_INSERT,
        Delete = ace.swig.eKeys.ACE_KEY_DELETE,
        Right = ace.swig.eKeys.ACE_KEY_RIGHT,
        Left = ace.swig.eKeys.ACE_KEY_LEFT,
        Down = ace.swig.eKeys.ACE_KEY_DOWN,
        PageUp = ace.swig.eKeys.ACE_KEY_PAGE_UP,
        PageDown = ace.swig.eKeys.ACE_KEY_PAGE_DOWN,
        Home = ace.swig.eKeys.ACE_KEY_HOME,
        End = ace.swig.eKeys.ACE_KEY_END,
        CapsLock = ace.swig.eKeys.ACE_KEY_CAPS_LOCK,
        ScrollLock = ace.swig.eKeys.ACE_KEY_SCROLL_LOCK,
        NumLock = ace.swig.eKeys.ACE_KEY_NUM_LOCK,
        PrintScreen = ace.swig.eKeys.ACE_KEY_PRINT_SCREEN,
        Pause = ace.swig.eKeys.ACE_KEY_PAUSE,
        F1 = ace.swig.eKeys.ACE_KEY_F1,
        F2 = ace.swig.eKeys.ACE_KEY_F2,
        F3 = ace.swig.eKeys.ACE_KEY_F3,
        F4 = ace.swig.eKeys.ACE_KEY_F4,
        F5 = ace.swig.eKeys.ACE_KEY_F5,
        F6 = ace.swig.eKeys.ACE_KEY_F6,
        F7 = ace.swig.eKeys.ACE_KEY_F7,
        F8 = ace.swig.eKeys.ACE_KEY_F8,
        F9 = ace.swig.eKeys.ACE_KEY_F9,
        F10 = ace.swig.eKeys.ACE_KEY_F10,
        F11 = ace.swig.eKeys.ACE_KEY_F11,
        F12 = ace.swig.eKeys.ACE_KEY_F12,
        F13 = ace.swig.eKeys.ACE_KEY_F13,
        F14 = ace.swig.eKeys.ACE_KEY_F14,
        F15 = ace.swig.eKeys.ACE_KEY_F15,
        F16 = ace.swig.eKeys.ACE_KEY_F16,
        F17 = ace.swig.eKeys.ACE_KEY_F17,
        F18 = ace.swig.eKeys.ACE_KEY_F18,
        F19 = ace.swig.eKeys.ACE_KEY_F19,
        F20 = ace.swig.eKeys.ACE_KEY_F20,
        F21 = ace.swig.eKeys.ACE_KEY_F21,
        F22 = ace.swig.eKeys.ACE_KEY_F22,
        F23 = ace.swig.eKeys.ACE_KEY_F23,
        F24 = ace.swig.eKeys.ACE_KEY_F24,
        F25 = ace.swig.eKeys.ACE_KEY_F25,
        KP0 = ace.swig.eKeys.ACE_KEY_KP_0,
        KP1 = ace.swig.eKeys.ACE_KEY_KP_1,
        KP2 = ace.swig.eKeys.ACE_KEY_KP_2,
        KP3 = ace.swig.eKeys.ACE_KEY_KP_3,
        KP4 = ace.swig.eKeys.ACE_KEY_KP_4,
        KP5 = ace.swig.eKeys.ACE_KEY_KP_5,
        KP6 = ace.swig.eKeys.ACE_KEY_KP_6,
        KP7 = ace.swig.eKeys.ACE_KEY_KP_7,
        KP8 = ace.swig.eKeys.ACE_KEY_KP_8,
        KP9 = ace.swig.eKeys.ACE_KEY_KP_9,
        KPDecimal = ace.swig.eKeys.ACE_KEY_KP_DECIMAL,
        KPDivide = ace.swig.eKeys.ACE_KEY_KP_DIVIDE,
        KPMultiply = ace.swig.eKeys.ACE_KEY_KP_MULTIPLY,
        KPSubtract = ace.swig.eKeys.ACE_KEY_KP_SUBTRACT,
        KPAdd = ace.swig.eKeys.ACE_KEY_KP_ADD,
        KPEnter = ace.swig.eKeys.ACE_KEY_KP_ENTER,
        KPEqual = ace.swig.eKeys.ACE_KEY_KP_EQUAL,
        LeftShit = ace.swig.eKeys.ACE_KEY_LEFT_SHIFT,
        LeftControl = ace.swig.eKeys.ACE_KEY_LEFT_CONTROL,
        LeftAlt = ace.swig.eKeys.ACE_KEY_LEFT_ALT,
        LeftSuper = ace.swig.eKeys.ACE_KEY_LEFT_SUPER,
        RightShift = ace.swig.eKeys.ACE_KEY_RIGHT_SHIFT,
        RightControl = ace.swig.eKeys.ACE_KEY_RIGHT_CONTROL,
        RightAlt = ace.swig.eKeys.ACE_KEY_RIGHT_ALT,
        RightSuper = ace.swig.eKeys.ACE_KEY_RIGHT_SUPER,
        Menu = ace.swig.eKeys.ACE_KEY_MENU,
        Last = ace.swig.eKeys.ACE_KEY_LAST,
        Max = ace.swig.eKeys.MAX
    };

    /// <summary>
    /// キーボードからの入力を取得するクラス
    /// </summary>
    public class Keyboard
    {
        private ace.swig.Keyboard keyboard;

        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <param name="kb">C++へのインタフェースクラス</param>
        internal Keyboard(ace.swig.Keyboard kb)
        {
            keyboard = kb;
        }

        /// <summary>
        /// 特定のキーの押下状態をKeyState列挙型で返す
        /// </summary>
        /// <param name="key">押下状態を調べたいキー</param>
        /// <returns>押下状態</returns>
        public KeyState GetKeyState(Keys key)
        {
            return (KeyState)keyboard.GetKeyState((swig.eKeys)key);
        }
    }
}
