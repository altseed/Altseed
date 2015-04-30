using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
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
        /// 特定のキーの押し状態をKeyState列挙型で返す
        /// </summary>
        /// <param name="key">押し状態を調べたいキー</param>
        /// <returns>押し状態</returns>
        public KeyState GetKeyState(Keys key)
        {
            return (KeyState)keyboard.GetKeyState((swig.Keys)key);
        }
    }
}
