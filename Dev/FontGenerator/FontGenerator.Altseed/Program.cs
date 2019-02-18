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
			var option = new asd.EngineOption()
			{
				IsReloadingEnabled = true
			};
			asd.Engine.Initialize("フォント生成ツール", 440, 640, option);
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
