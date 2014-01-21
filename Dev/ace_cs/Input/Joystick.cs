using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
    /// <summary>
    /// ジョイスティックの押下状態を示す列挙型
    /// </summary>
    public enum JoystickButtonState
    {
        /// <summary>
        /// キーを離した瞬間のみ返す
        /// </summary>
        Pull=ace.swig.eJoystickButtonState.ACE_JOYSTICK_BUTTON_PULL,
        /// <summary>
        /// キーを押した瞬間のみ返す
        /// </summary>
        Push=ace.swig.eJoystickButtonState.ACE_JOYSTICK_BUTTON_PUSH,
        /// <summary>
        /// キーを押しっぱなしの時に返す
        /// </summary>
        Hold=ace.swig.eJoystickButtonState.ACE_JOYSTICK_BUTTON_HOLD,
        /// <summary>
        /// キーを離しっぱなしの時に返す
        /// </summary>
        Free=ace.swig.eJoystickButtonState.ACE_JOYSTICK_BUTTON_FREE
    }

    public class Joystick
    {
        private ace.swig.Joystick joystick;

        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <param name="js">ネイティブインタフェース</param>
        internal Joystick(ace.swig.Joystick js)
        {
            joystick = js;
        }

        /// <summary>
        /// ジョイスティックの名前を返す
        /// </summary>
        public string JoystickName
        {
            get
            {
                return joystick.GetJoystickName();
            }
        }
        
        /// <summary>
        /// ジョイスティックのボタンの合計数を返す
        /// </summary>
        public int ButtonsCount
        {
            get
            {
                return joystick.GetButtonsCount();
            }
        }

        /// <summary>
        /// ジョイスティックの軸の合計数を返す
        /// </summary>
        public int AxesCount
        {
            get
            {
                return joystick.GetAxesCount();
            }
        }

        /// <summary>
        /// あるボタンの押下状態を返す
        /// </summary>
        /// <param name="at">何番目のボタンの押下状態を取得するか</param>
        /// <returns>押下状態</returns>
        public JoystickButtonState GetButtonState(int at)
        {
            return (JoystickButtonState)joystick.GetButtonState(at);
        }

        /// <summary>
        /// ある軸の入力状態を返す
        /// </summary>
        /// <param name="at">何番目の軸配列の状態を取得するか</param>
        /// <returns>傾倒の度合い</returns>
        public float GetAxisState(int at)
        {
            return joystick.GetAxisState(at);
        }
    }
}
