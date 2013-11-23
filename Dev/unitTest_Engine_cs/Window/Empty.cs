using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using NUnit.Framework;

namespace unitTest_Engine_cs.Window
{
	[TestFixture]
	public class Empty
	{
		[Test]
		public void Test()
		{
			var option = new ace.EngineOption
			{
				GraphicsType = ace.GraphicsType.DirectX11,
				IsFullScreen = false
			};
			var initialized = ace.Engine.Initialize("test", 640, 480, option);

			int time = 0;

			while (ace.Engine.DoEvents())
			{
				ace.Engine.Update();

				if (time == 10) break;
				time++;
			}

			ace.Engine.Terminate();
		}
	}
}
