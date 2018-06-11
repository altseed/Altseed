using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FontGenerator.Altseed
{
	class Program
	{
		[STAThread]
		static void Main(string[] args)
		{
			asd.Engine.Initialize("フォント生成ツール", 320, 640, new asd.EngineOption());
			asd.Engine.OpenTool();
            
			var tool = new ToolRenderer();

			while (asd.Engine.DoEvents())
			{
				// must to run before Engine.Update (TODO fix it or throw exception)
				tool.Render();
				asd.Engine.Update();
			}

			asd.Engine.CloseTool();
			asd.Engine.Terminate();
		}
	}
}
