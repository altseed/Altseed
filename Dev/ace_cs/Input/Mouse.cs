using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
    /// <summary>
    /// マウスからの入力を取得するクラス
    /// </summary>
    public class Mouse
    {
        private ace.swig.Mouse mouse;
        private SideButton leftButton;
        private SideButton rightButton;
        private MiddleButton middleButton;

        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <param name="m">ネイティブインタフェース</param>
        internal Mouse(ace.swig.Mouse m)
        {
            mouse = m;
            leftButton = new SideButton();
            rightButton = new SideButton();
            middleButton = new MiddleButton();
        }

        internal void RefreshAllState()
        {
            MouseButtonState mstate = (MouseButtonState)mouse.GetLeftButton().GetButtonState();
            leftButton.SetState(mstate);

            mstate = (MouseButtonState)mouse.GetRightButton().GetButtonState();
            rightButton.SetState(mstate);

            mstate = (MouseButtonState)mouse.GetMiddleButton().GetButtonState();
            double rot = mouse.GetMiddleButton().GetRotation();
            middleButton.SetState(mstate, rot);
        }

        /// <summary>
        /// マウスの現在位置を返す
        /// </summary>
        public Vector2DF Position
        {
            get
            {
                return mouse.GetPosition();
            }
        }

        /// <summary>
        /// 左ボタンの状態を返すプロパティ
        /// </summary>
        public SideButton LeftButton {
            get
            {
                return leftButton;
            }
        }

        /// <summary>
        /// 右ボタンの状態を返すプロパティ
        /// </summary>
        public SideButton RightButton
        {
            get
            {
                return rightButton;
            }
        }

        /// <summary>
        /// 中ボタンの状態を返すプロパティ
        /// </summary>
        public MiddleButton MiddleButton
        {
            get
            {
                return middleButton;
            }
        }
    }
}
