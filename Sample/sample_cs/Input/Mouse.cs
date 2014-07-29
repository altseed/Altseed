using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace sample_cs.Input
{
    class Mouse:ISample
    {
        [STAThread]
        public void Run()
        {
            // aceを初期化する。
            var initialized = ace.Engine.Initialize("Mouse", 640, 480, new ace.EngineOption());

            //aceが続行可能か調べる。
            while (ace.Engine.DoEvents())
            {
                //aceを更新する。
                ace.Engine.Update();

                //マウスへの参照を取得する。
                ace.Mouse mouse = ace.Engine.Mouse;

                //マウスカーソルの座標を取得して表示する。
                {
                    ace.Vector2DF v = mouse.Position;

                    Console.Write("Cursor Position:({0},{1}) ", v.X, v.Y);
                }

                //マウスの中央ボタンの押下状態を表示する。
                switch (mouse.MiddleButton.ButtonState)
                {
                    case ace.MouseButtonState.Free: //前フレーム、現フレーム共に非押下。
                        Console.Write("Middle Free");
                        break;
                    case ace.MouseButtonState.Hold: //前フレーム、現フレーム共に押下。
                        Console.Write("Middle Hold");
                        break;
                    case ace.MouseButtonState.Pull: //前フレームで押下、現フレームで非押下。
                        Console.Write("Middle Pull");
                        break;
                    case ace.MouseButtonState.Push: //前フレームで非押下、現フレームで押下。
                        Console.Write("Middle Push");
                        break;
                }

                //マウスのホイールの回転状態を取得して表示する。
                {
                    Console.WriteLine(" Wheel : "+ mouse.MiddleButton.WheelRotation);
                }
                
            }
            //aceを終了する。
            ace.Engine.Terminate();
        }
    }
}
