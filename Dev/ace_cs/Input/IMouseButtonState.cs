using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
    /// <summary>
    /// マウスの押下状態を示す列挙型
    /// </summary>
    public enum MouseButtonState
    {
        /// <summary>
        /// ボタンを離した瞬間のみ返す
        /// </summary>
        Pull=ace.swig.eMouseButtonState.MOUSE_PULL,
        /// <summary>
        /// ボタンを押した瞬間のみ返す
        /// </summary>
        Push=ace.swig.eMouseButtonState.MOUSE_PUSH,
        /// <summary>
        /// ボタンを押していない時に返す
        /// </summary>
        Free=ace.swig.eMouseButtonState.MOUSE_FREE,
        /// <summary>
        /// ボタンを押している時に返す
        /// </summary>
        Hold=ace.swig.eMouseButtonState.MOUSE_HOLD
    }

    /// <summary>
    /// マウスのボタン押下状態のインタフェース
    /// </summary>
    public interface IMouseButtonState
    {
        /// <summary>
        /// ボタンの押下状態を返す
        /// </summary>
        MouseButtonState ButtonState { get; } 
    }
}
