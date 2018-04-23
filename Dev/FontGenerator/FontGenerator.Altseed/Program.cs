using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FontGenerator.Altseed
{
	class Program
	{
		static void Main(string[] args)
		{
			asd.Engine.Initialize("フォント生成ツール", 320, 640, new asd.EngineOption());
			asd.Engine.OpenTool();

			var tool = new ToolRenderer();

			while (asd.Engine.DoEvents())
			{
				asd.Engine.Update();
				tool.Render();
			}

			asd.Engine.CloseTool();
			asd.Engine.Terminate();
		}
	}
}
