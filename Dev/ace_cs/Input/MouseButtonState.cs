using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
    /// <summary>
    /// マウスボタンの押し状態を示す列挙型
    /// </summary>
    public enum MouseButtonState
    {
        /// <summary>
        /// ボタンをこのフレームで離した状態
        /// </summary>
        Release =ace.swig.MouseButtonState.Release,
        /// <summary>
        /// ボタンをこのフレームで押した状態
        /// </summary>
        Push = ace.swig.MouseButtonState.Push,
        /// <summary>
        /// ボタンを離し続けている状態
        /// </summary>
        Free =ace.swig.MouseButtonState.Free,
        /// <summary>
        /// ボタンを押し続けている状態
        /// </summary>
        Hold =ace.swig.MouseButtonState.Hold
    }
}
