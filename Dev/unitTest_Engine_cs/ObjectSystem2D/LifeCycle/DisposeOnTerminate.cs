using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using asd;

namespace unitTest_Engine_cs.ObjectSystem2D.LifeCycle
{
	class DisposeOnTerminate : TestFramework
	{
		class TestScene : Scene
		{
			public bool AlreadyDisposed = false;

			protected override void OnDispose()
			{
				AlreadyDisposed = true;
			}
		}

		public override void Test(GraphicsDeviceType graphicsType)
		{
			var option = new EngineOption()
			{
				GraphicsDevice = graphicsType
			};
			Engine.Initialize("DisposeOnTerminate", 640, 480, option);

			var scene = new TestScene();
			Engine.ChangeScene(scene);

			while (Engine.DoEvents())
			{
				Engine.Update();
			}

			Engine.Terminate();

			Assert.AreEqual(true, scene.AlreadyDisposed);
		}
	}
}
