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
    public partial class Keyboard
    {
        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <param name="kb">C++へのインタフェースクラス</param>
        internal Keyboard(ace.swig.Keyboard kb)
        {
			CoreInstance = kb;
        }
    }
}
