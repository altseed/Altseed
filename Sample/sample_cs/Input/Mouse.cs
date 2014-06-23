using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace test_cs.Input
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

                //マウスの中央ボタンの押下状態を表示する。
                switch (mouse.MiddleButton.ButtonState)
                {
                    case ace.MouseButtonState.Free: //前フレーム、現フレーム共に非押下。
                        Console.WriteLine("Middle Free");
                        break;
                    case ace.MouseButtonState.Hold: //前フレーム、現フレーム共に押下。
                        Console.WriteLine("Middle Hold");
                        break;
                    case ace.MouseButtonState.Pull: //前フレームで押下、現フレームで非押下。
                        Console.WriteLine("Middle Pull");
                        break;
                    case ace.MouseButtonState.Push: //前フレームで非押下、現フレームで押下。
                        Console.WriteLine("Middle Push");
                        break;
                }

                /*
                {
                    //マウスのカーソルの位置を表示する。
                    Console.WriteLine("(" + mouse.Position.X + "," + mouse.Position.Y + ")");
                }
                */
            }
            //aceを終了する。
            ace.Engine.Terminate();
        }
    }
}
