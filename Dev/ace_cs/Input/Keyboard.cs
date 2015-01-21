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
        ///Release-キーを離した瞬間のみ返す
        /// </summary>
        Release = ace.swig.KeyState.Release,
        /// <summary>
        /// Push-キーを押した瞬間のみ返す
        /// </summary>
        Push = ace.swig.KeyState.Push,
        /// <summary>
        /// Free-キーを押していない時に返す
        /// </summary>
        Free = ace.swig.KeyState.Free,
        /// <summary>
        /// Hold-キーを押している時に返す
        /// </summary>
        Hold = ace.swig.KeyState.Hold
    };

    /// <summary>
    /// ACEで扱うキーボードのキー一覧
    /// </summary>
    public enum Keys : int
    {
        Unknown = ace.swig.Keys.Unknown,
        Space = ace.swig.Keys.Space,
        Apostrophe = ace.swig.Keys.Apostrophe,
        Comma = ace.swig.Keys.Comma,
        Minus = ace.swig.Keys.Minus,
        Period = ace.swig.Keys.Period,
        Slash = ace.swig.Keys.Slash,
        Num0 = ace.swig.Keys.Num0,
        Num1 = ace.swig.Keys.Num1,
        Num2 = ace.swig.Keys.Num2,
        Num3 = ace.swig.Keys.Num3,
        Num4 = ace.swig.Keys.Num4,
        Num5 = ace.swig.Keys.Num5,
        Num6 = ace.swig.Keys.Num6,
        Num7 = ace.swig.Keys.Num7,
        Num8 = ace.swig.Keys.Num8,
        Num9 = ace.swig.Keys.Num9,
        Semicolon = ace.swig.Keys.Semicolon,
        Equal = ace.swig.Keys.Equal,
        A = ace.swig.Keys.A,
        B = ace.swig.Keys.B,
        C = ace.swig.Keys.C,
        D = ace.swig.Keys.D,
        E = ace.swig.Keys.E,
        F = ace.swig.Keys.F,
        G = ace.swig.Keys.G,
        H = ace.swig.Keys.H,
        I = ace.swig.Keys.I,
        J = ace.swig.Keys.J,
        K = ace.swig.Keys.K,
        L = ace.swig.Keys.L,
        M = ace.swig.Keys.M,
        N = ace.swig.Keys.N,
        O = ace.swig.Keys.O,
        P = ace.swig.Keys.P,
        Q = ace.swig.Keys.Q,
        R = ace.swig.Keys.R,
        S = ace.swig.Keys.S,
        T = ace.swig.Keys.T,
        U = ace.swig.Keys.U,
        V = ace.swig.Keys.V,
        W = ace.swig.Keys.W,
        X = ace.swig.Keys.X,
        Y = ace.swig.Keys.Y,
        Z = ace.swig.Keys.Z,
        LeftBracket = ace.swig.Keys.LeftBracket,
        Backslash = ace.swig.Keys.Backslash,
        RightBracket = ace.swig.Keys.RightBracket,
        GraveAccent = ace.swig.Keys.GraveAccent,
        World1 = ace.swig.Keys.World1,
        World2 = ace.swig.Keys.World2,
        Escape = ace.swig.Keys.Escape,
        Enter = ace.swig.Keys.Enter,
        Tab = ace.swig.Keys.Tab,
        Backspace = ace.swig.Keys.Backspace,
        Insert = ace.swig.Keys.Insert,
        Delete = ace.swig.Keys.Delete,
        Right = ace.swig.Keys.Right,
        Left = ace.swig.Keys.Left,
        Down = ace.swig.Keys.Down,
        Up = ace.swig.Keys.Up,
        PageUp = ace.swig.Keys.PageUp,
        PageDown = ace.swig.Keys.PageDown,
        Home = ace.swig.Keys.Home,
        End = ace.swig.Keys.End,
        CapsLock = ace.swig.Keys.CapsLock,
        ScrollLock = ace.swig.Keys.ScrollLock,
        NumLock = ace.swig.Keys.NumLock,
        PrintScreen = ace.swig.Keys.PrintScreen,
        Pause = ace.swig.Keys.Pause,
        F1 = ace.swig.Keys.F1,
        F2 = ace.swig.Keys.F2,
        F3 = ace.swig.Keys.F3,
        F4 = ace.swig.Keys.F4,
        F5 = ace.swig.Keys.F5,
        F6 = ace.swig.Keys.F6,
        F7 = ace.swig.Keys.F7,
        F8 = ace.swig.Keys.F8,
        F9 = ace.swig.Keys.F9,
        F10 = ace.swig.Keys.F10,
        F11 = ace.swig.Keys.F11,
        F12 = ace.swig.Keys.F12,
        F13 = ace.swig.Keys.F13,
        F14 = ace.swig.Keys.F14,
        F15 = ace.swig.Keys.F15,
        F16 = ace.swig.Keys.F16,
        F17 = ace.swig.Keys.F17,
        F18 = ace.swig.Keys.F18,
        F19 = ace.swig.Keys.F19,
        F20 = ace.swig.Keys.F20,
        F21 = ace.swig.Keys.F21,
        F22 = ace.swig.Keys.F22,
        F23 = ace.swig.Keys.F23,
        F24 = ace.swig.Keys.F24,
        F25 = ace.swig.Keys.F25,
        Keypad0 = ace.swig.Keys.Keypad0,
        Keypad1 = ace.swig.Keys.Keypad1,
        Keypad2 = ace.swig.Keys.Keypad2,
        Keypad3 = ace.swig.Keys.Keypad3,
        Keypad4 = ace.swig.Keys.Keypad4,
        Keypad5 = ace.swig.Keys.Keypad5,
        Keypad6 = ace.swig.Keys.Keypad6,
        Keypad7 = ace.swig.Keys.Keypad7,
        Keypad8 = ace.swig.Keys.Keypad8,
        Keypad9 = ace.swig.Keys.Keypad9,
        KeypadDecimal = ace.swig.Keys.KeypadDecimal,
        KeypadDivide = ace.swig.Keys.KeypadDivide,
        KeypadSubstract = ace.swig.Keys.KeypadSubstract,
        KeypadAdd = ace.swig.Keys.KeypadAdd,
        KeypadEnter = ace.swig.Keys.KeypadEnter,
        KeypadEqual = ace.swig.Keys.KeypadEqual,
        LeftShift = ace.swig.Keys.LeftShift,
        LeftControl = ace.swig.Keys.LeftControl,
        LeftAlt = ace.swig.Keys.LeftAlt,
        LeftWin = ace.swig.Keys.LeftWin,
        RightShift = ace.swig.Keys.RightShift,
        RightControl = ace.swig.Keys.RightControl,
        RightAlt = ace.swig.Keys.RightAlt,
        RightWin = ace.swig.Keys.RightWin,
        Menu = ace.swig.Keys.Menu,
        Last = ace.swig.Keys.Last,
        MAX = ace.swig.Keys.MAX
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
            return (KeyState)keyboard.GetKeyState((swig.Keys)key);
        }
    }
}
