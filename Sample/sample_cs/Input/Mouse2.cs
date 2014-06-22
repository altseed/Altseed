using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace test_cs.Input
{
    class Mouse2:ISample
    {
        [STAThread]
        public void Run()
        {
            // aceを初期化する。
            var initialized = ace.Engine.Initialize("Mouse2", 640, 480, new ace.EngineOption());

            //aceが続行可能か調べる。
            while (ace.Engine.DoEvents())
            {
                //aceを更新する。
                ace.Engine.Update();

                //マウスへの参照を取得する。
                ace.Mouse mouse = ace.Engine.Mouse;

                //マウスのカーソルの位置を表示する。
                Console.WriteLine("(" + mouse.Position.X + "," + mouse.Position.Y + ")");
            }
            //aceを終了する。
            ace.Engine.Terminate();
        }
    }
}
