using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace test_cs
{
    class MouseSample2:ISample
    {
        [STAThread]
        public void Run()
        {
            var initialized = ace.Engine.Initialize("Mouse2", 640, 480, false);

            while (ace.Engine.DoEvents())
            {
                ace.Engine.Update();

                ace.Mouse mouse = ace.Engine.Mouse;

                Console.WriteLine("(" + mouse.Position.X + "," + mouse.Position.Y + ")");
            }

            ace.Engine.Terminate();
        }
    }
}
