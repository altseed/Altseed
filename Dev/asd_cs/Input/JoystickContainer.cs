using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
    public partial class JoystickContainer
    {
        private const int MAX_JOYSTICK_NUM=16;
        private Joystick[] joysticks;

        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <param name="jc">ネイティブインタフェース</param>
        internal JoystickContainer(asd.swig.JoystickContainer jc)
        {
            CoreInstance = jc;
            joysticks = new Joystick[MAX_JOYSTICK_NUM];
            for(int i=0;i<MAX_JOYSTICK_NUM;++i)
            {
                joysticks[i] = new Joystick(CoreInstance.GetJoystickAt(i));
            }
        }

        /// <summary>
        /// 引数に指定した番号のジョイスティッククラスを返す。
        /// </summary>
        /// <param name="at">何番目のジョイスティッククラスを取得するか</param>
        /// <returns>該当するジョイスティッククラス</returns>
        public asd.Joystick GetJoystickAt(int at)
        {
            return joysticks[at];
        }
    }
}
