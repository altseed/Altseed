using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
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
        /// 指定したボタンの押し状態を返す
        /// </summary>
        /// <param name="at">何番目のボタンの押し状態を取得するか</param>
        /// <returns>押し状態</returns>
        public JoystickButtonState GetButtonState(int at)
        {
            return (JoystickButtonState)joystick.GetButtonState(at);
        }

        /// <summary>
        /// 指定した軸の倒し具合を返す
        /// </summary>
        /// <param name="at">何番目の軸配列の倒し具合を取得するか</param>
        /// <returns>倒し具合</returns>
        public float GetAxisState(int at)
        {
            return joystick.GetAxisState(at);
        }
    }
}
