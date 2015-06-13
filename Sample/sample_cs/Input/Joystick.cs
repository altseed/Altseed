using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace sample_cs.Input
{
    class Joystick:ISample
    {
        [STAThread]
        public void Run()
        {
			// aceを初期化する。
            var initialized = asd.Engine.Initialize("Joystick", 640, 480, new asd.EngineOption());

            asd.JoystickContainer container = asd.Engine.JoystickContainer;

            //0番目のジョイスティックが接続されているか否かを確かめる
            if (container.GetIsPresentAt(0))
            {
                //接続されている場合は、ジョイスティックの名前を取得して表示する。
                Console.WriteLine(container.GetJoystickAt(0).JoystickName + " was connected.");
            }
            else
            {
                //接続されていない場合は、テストを終了する。
                Console.WriteLine("No joystick are connected."); 
                Console.WriteLine("Hit any key to finish test.");
                Console.ReadKey();
                System.Environment.Exit(1);
            }

            Console.WriteLine("Hit any key to continue test.");
            Console.ReadKey();

            //aceが続行可能か調べる。
            while (asd.Engine.DoEvents())
            {
                //aceを更新する。
                asd.Engine.Update();

                //0番目のジョイスティックの0番目のボタンの押下状態を確かめる
                switch (container.GetJoystickAt(0).GetButtonState(0))
                {
                    case asd.JoystickButtonState.Free: //前フレーム、現フレーム共に非押下。
                        Console.WriteLine("0 Free");
                        break;
                    case asd.JoystickButtonState.Hold: //前フレーム、現フレーム共に押下。
                        Console.WriteLine("0 Hold");
                        break;
                    case asd.JoystickButtonState.Release: //前フレームで押下、現フレームで非押下。
                        Console.WriteLine("0 Release");
                        break;
                    case asd.JoystickButtonState.Push: //前フレームで非押下、現フレームで押下。
                        Console.WriteLine("0 Push");
                        break;
                }
            }
            //aceを終了する。
            asd.Engine.Terminate();
        }
    }
}
