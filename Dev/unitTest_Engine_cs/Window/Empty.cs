using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

//using NUnit.Framework;

namespace unitTest_Engine_cs.Window
{
	//[TestFixture]
	public class Empty : TestFramework
	{
		//[Test]
		public override void Test(asd.GraphicsDeviceType graphicsType)
		{
			var option = new asd.EngineOption
			{
				GraphicsDevice = graphicsType,
				IsFullScreen = false
			};

			var initialized = asd.Engine.Initialize("Empty", 640, 480, option);

			int time = 0;

			while (asd.Engine.DoEvents())
			{
				asd.Engine.Update();

				if (time == 10) break;
				time++;
			}

			asd.Engine.Terminate();
		}
	}
}
