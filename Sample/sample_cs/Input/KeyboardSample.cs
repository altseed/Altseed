using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace test_cs.Input
{
    class KeyboardSample:ISample
    {
        [STAThread]
        public void Run()
        {
			// 初期設定を行う。
			var option = new ace.EngineOption
			{
				GraphicsType = ace.GraphicsType.DirectX11,
				IsFullScreen = false
			};

            var initialized = ace.Engine.Initialize("Keyboard", 640, 480, option);

            while (ace.Engine.DoEvents())
            {
                ace.Engine.Update();

                ace.Keyboard keyboard = ace.Engine.Keyboard;

                switch (keyboard.GetKeyState(ace.Keys.Z))
                {
                    case ace.KeyState.Free:
                        Console.WriteLine("Z Free");
                        break;
                    case ace.KeyState.Hold:
                        Console.WriteLine("Z Hold");
                        break;
                    case ace.KeyState.Pull:
                        Console.WriteLine("Z Pull");
                        break;
                    case ace.KeyState.Push:
                        Console.WriteLine("Z Push");
                        break;
                }
            }

            ace.Engine.Terminate();
        }
    }
}
