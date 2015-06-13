using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace sample_cs.Input
{
    class Keyboard:ISample
    {
        [STAThread]
        public void Run()
        {
			//aceを初期化する。
            var initialized = asd.Engine.Initialize("Keyboard", 640, 480, new asd.EngineOption());

            //aceが続行可能か調べる。
            while (asd.Engine.DoEvents())
            {
                //aceを更新する。
                asd.Engine.Update();

                //キーボードへの参照を取得する。
                asd.Keyboard keyboard = asd.Engine.Keyboard;

                //Zキーの押下状態を表示する。
                switch (keyboard.GetKeyState(asd.Keys.Z))
                {
                    case asd.KeyState.Free: //前フレーム、現フレーム共に非押下。
                        Console.WriteLine("Z Free");
                        break;
                    case asd.KeyState.Hold: //前フレーム、現フレーム共に押下。
                        Console.WriteLine("Z Hold");
                        break;
                    case asd.KeyState.Release: //前フレームで押下、現フレームで非押下。
                        Console.WriteLine("Z Release");
                        break;
                    case asd.KeyState.Push: //前フレームで非押下、現フレームで押下。
                        Console.WriteLine("Z Push");
                        break;
                }
            }
            //aceを終了する。
            asd.Engine.Terminate();
        }
    }
}
