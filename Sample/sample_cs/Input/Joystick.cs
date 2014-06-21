using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace test_cs.Input
{
    class Joystick:ISample
    {
        [STAThread]
        public void Run()
        {
			// 初期設定を行う。
            var initialized = ace.Engine.Initialize("Joystick", 640, 480, new ace.EngineOption());

            ace.JoystickContainer container = ace.Engine.JoystickContainer;

            //0番目のジョイスティックが接続されているか否かを確かめる
            if(container.IsPresentAt(0))
            {
                Console.WriteLine(container.GetJoystickAt(0).JoystickName + " was connected.");
            }
            else
            {
                Console.WriteLine("No joystick are connected."); 
                Console.WriteLine("Hit any key to finish test.");
                Console.ReadKey();
                System.Environment.Exit(1);
            }

            Console.WriteLine("Hit any key to continue test.");
            Console.ReadKey();
            
            //0番目のジョイスティックの0番目のボタンの押下状態を確かめる
            while (ace.Engine.DoEvents())
            {
                ace.Engine.Update();

                switch (container.GetJoystickAt(0).GetButtonState(0))
                {
                    case ace.JoystickButtonState.Free:
                        Console.WriteLine("0 Free");
                        break;
                    case ace.JoystickButtonState.Hold:
                        Console.WriteLine("0 Hold");
                        break;
                    case ace.JoystickButtonState.Pull:
                        Console.WriteLine("0 Pull");
                        break;
                    case ace.JoystickButtonState.Push:
                        Console.WriteLine("0 Push");
                        break;
                }
            }

            ace.Engine.Terminate();
        }
    }
}
