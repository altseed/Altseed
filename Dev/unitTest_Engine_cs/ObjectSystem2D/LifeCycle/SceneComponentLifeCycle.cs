using System;
using asd;

namespace unitTest_Engine_cs.ObjectSystem2D.LifeCycle
{
	class SceneComponentLifeCycle : EngineTest
	{
		private class MyComponent : asd.SceneComponent
		{
			public int State { get; private set; }

			public MyComponent()
			{
				State = 0;
			}

			protected override void OnRegistered()
			{
				Assert.AreEqual(0, State);
				State = 1;
			}

			protected override void OnStartUpdating()
			{
				Assert.AreEqual(1, State);
				State = 2;
			}

			protected override void OnTransitionFinished()
			{
				Assert.AreEqual(2, State);
				State = 3;
			}

			protected override void OnTransitionBegin()
			{
				Assert.AreEqual(3, State);
				State = 4;
			}

			protected override void OnStopUpdating()
			{
				Assert.AreEqual(4, State);
				State = 5;
			}

			protected override void OnUnregistered()
			{
				Assert.AreEqual(5, State);
				State = 0;
			}
		}

		public SceneComponentLifeCycle() : base(60)
		{
		}

		protected override void OnStart()
		{
			var scene = new Scene();
			var layer = new Layer2D();
			var obj = new TextureObject2D
			{
				Texture = Engine.Graphics.CreateTexture2D(CloudTexturePath)
			};

			scene.AddComponent(new MyComponent(), "LifeCycle");

			layer.AddObject(obj);
			scene.AddLayer(layer);
			Engine.ChangeSceneWithTransition(scene, new TransitionFade(0.2f, 0.2f));
		}

		protected override void OnUpdated()
		{
			if (Time == 30)
			{
				var component = Engine.CurrentScene.GetComponent("LifeCycle") as MyComponent;
				Assert.AreEqual(3, component.State);
				Console.WriteLine("ChangeScene");
				Engine.ChangeSceneWithTransition(new Scene(), new TransitionFade(0.2f, 0.2f));
			}
		}
	}
}
