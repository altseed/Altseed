using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
    /// <summary>
    /// マウスの中ボタンの状態を扱うクラス
    /// </summary>
    public class MiddleButton
    {
        /// <summary>
        /// 回転度合いを保持する変数
        /// </summary>
        private double rotation;

        /// <summary>
        /// 押下状態を保持する変数
        /// </summary>
        private MouseButtonState mouseButtonState;

        /// <summary>
        /// コンストラクタ
        /// </summary>
        internal MiddleButton() { }

        /// <summary>
        /// 押し状態とホイール回転状態をセットする関数
        /// </summary>
        /// <param name="mbuttonstate">押し状態</param>
        /// <param name="rot">回転度合い</param>
        internal void SetState(MouseButtonState mbuttonstate,double rot)
        {
            rotation = rot;
            mouseButtonState = mbuttonstate;
        }

        /// <summary>
        /// ホイールの回転度合いを返すプロパティ
        /// </summary>
        public double WheelRotation
        {
            get { return rotation; }
        }

        /// <summary>
        /// 押し状態を返すプロパティ
        /// </summary>
        public MouseButtonState ButtonState
        {
            get { return mouseButtonState; }
        }
    }
}
