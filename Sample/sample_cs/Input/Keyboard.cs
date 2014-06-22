using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace test_cs.Input
{
    class Keyboard:ISample
    {
        [STAThread]
        public void Run()
        {
			//aceを初期化する。
            var initialized = ace.Engine.Initialize("Keyboard", 640, 480, new ace.EngineOption());

            //aceが続行可能か調べる。
            while (ace.Engine.DoEvents())
            {
                //aceを更新する。
                ace.Engine.Update();

                //キーボードへの参照を取得する。
                ace.Keyboard keyboard = ace.Engine.Keyboard;

                //Zキーの押下状態を表示する。
                switch (keyboard.GetKeyState(ace.Keys.Z))
                {
                    case ace.KeyState.Free: //前フレーム、現フレーム共に非押下。
                        Console.WriteLine("Z Free");
                        break;
                    case ace.KeyState.Hold: //前フレーム、現フレーム共に押下。
                        Console.WriteLine("Z Hold");
                        break;
                    case ace.KeyState.Pull: //前フレームで押下、現フレームで非押下。
                        Console.WriteLine("Z Pull");
                        break;
                    case ace.KeyState.Push: //前フレームで非押下、現フレームで押下。
                        Console.WriteLine("Z Push");
                        break;
                }
            }
            //aceを終了する。
            ace.Engine.Terminate();
        }
    }
}
