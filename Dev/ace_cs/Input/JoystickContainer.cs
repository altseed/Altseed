using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
    public class JoystickContainer
    {
        private ace.swig.JoystickContainer joystickContainer;
        private const int MAX_JOYSTICK_NUM=16;
        private Joystick[] joysticks;

        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <param name="jc">ネイティブインタフェース</param>
        internal JoystickContainer(ace.swig.JoystickContainer jc)
        {
            joystickContainer = jc;
            joysticks = new Joystick[MAX_JOYSTICK_NUM];
            for(int i=0;i<MAX_JOYSTICK_NUM;++i)
            {
                joysticks[i] = new Joystick(joystickContainer.GetJoystickAt(i));
            }
        }

        /// <summary>
        /// 引数に指定した番号のジョイスティックが接続されているかを取得する。
        /// </summary>
        /// <param name="at">何番目のジョイスティックの接続を確認するか</param>
        /// <returns>接続されているか否か</returns>
        public bool IsPresentAt(int at)
        {
			return joystickContainer.GetIsPresentAt(at);
        }

        /// <summary>
        /// 引数に指定した番号のジョイスティッククラスを返す。
        /// </summary>
        /// <param name="at">何番目のジョイスティッククラスを取得するか</param>
        /// <returns>該当するジョイスティッククラス</returns>
        public ace.Joystick GetJoystickAt(int at)
        {
            return joysticks[at];
        }

        /// <summary>
        /// 全てのジョイスティックの接続状態を更新する。
        /// </summary>
        public void RefreshAllJoysticks()
        {
            joystickContainer.RefreshAllJoysticks();
        }
    }
}
