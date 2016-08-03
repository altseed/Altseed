using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using asd;

namespace unitTest_Engine_cs.ObjectSystem2D.LifeCycle
{
	class SceneLifeCycle : EngineTest
	{
		class TestScene : Scene
		{
			private int phase = 0;
			public bool alreadyDisposed = false;

			protected override void OnRegistered()
			{
				Assert.AreEqual(0, phase);
				phase = 1;
			}

			protected override void OnStartUpdating()
			{
				Assert.AreEqual(1, phase);
				phase = 2;
			}

			protected override void OnTransitionFinished()
			{
				Assert.AreEqual(2, phase);
				phase = 3;
			}

			protected override void OnTransitionBegin()
			{
				Assert.AreEqual(3, phase);
				phase = 4;
			}

			protected override void OnStopUpdating()
			{
				Assert.AreEqual(4, phase);
				phase = 5;
			}

			protected override void OnUnregistered()
			{
				Assert.AreEqual(5, phase);
				phase = 6;
			}

			protected override void OnDispose()
			{
				alreadyDisposed = true;
			}
		}

		private TestScene scene1;
		private TestScene scene2;
		private TestScene scene3;

		public SceneLifeCycle() : base(180)
		{
		}

		protected override void OnStart()
		{
			scene1 = new TestScene();
			Engine.ChangeSceneWithTransition(scene1, new TransitionFade(0.3f, 0.3f));
		}

		protected override void OnUpdating()
		{
			if (Time == 60)
			{
				scene2 = new TestScene();
				Engine.ChangeSceneWithTransition(scene2, new TransitionFade(0.3f, 0.3f));
			}
			else if (Time == 120)
			{
				Assert.AreEqual(true, scene1.alreadyDisposed);
				scene3 = new TestScene();
				Engine.ChangeScene(scene3);
			}
			else if (Time == 122)
			{
				Assert.AreEqual(true, scene2.alreadyDisposed);
				scene3.Dispose();
			}
			else if (Time == 123)
			{
				Assert.AreEqual(true, scene3.alreadyDisposed);
			}
		}
	}
}
