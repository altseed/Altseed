using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace test_cs.Input
{
    class MouseSample1:ISample
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

            var initialized = ace.Engine.Initialize("Mouse1", 640, 480, option);

            while (ace.Engine.DoEvents())
            {
                ace.Engine.Update();

                ace.Mouse mouse = ace.Engine.Mouse;

                switch (mouse.MiddleButton.ButtonState)
                {
                    case ace.MouseButtonState.Free:
                        Console.WriteLine("Middle Free");
                        break;
                    case ace.MouseButtonState.Hold:
                        Console.WriteLine("Middle Hold");
                        break;
                    case ace.MouseButtonState.Pull:
                        Console.WriteLine("Middle Pull");
                        break;
                    case ace.MouseButtonState.Push:
                        Console.WriteLine("Middle Push");
                        break;
                }
            }

            ace.Engine.Terminate();
        }
    }
}
