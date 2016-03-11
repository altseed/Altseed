using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using asd;

namespace unitTest_Engine_cs.ObjectSystem2D
{
	class LifeCycle : EngineTest
	{
		class MessageObject : TextureObject2D
		{
			public bool IsAdded { get; private set; }
			public bool HaveBeenDisposed { get; private set; }

			public MessageObject()
			{
				Texture = Engine.Graphics.CreateTexture2D(CloudTexturePath);
				IsAdded = false;
				HaveBeenDisposed = false;
			}

			protected override void OnAdded()
			{
				IsAdded = true;
			}

			protected override void OnRemoved()
			{
				IsAdded = false;
			}

			protected override void OnDispose()
			{
				HaveBeenDisposed = true;
			}

			protected override void OnUpdate()
			{
				Angle += 2;
			}

			protected override void OnDrawAdditionally()
			{
				DrawCircleAdditionally(Position, 50, 10, new Color(0, 255, 0), 12, 0, null, AlphaBlendMode.Blend, 2);
			}
		}

		class MessageLayer : Layer2D
		{
			public bool IsAdded { get; private set; }
			public bool HaveBeenDisposed { get; private set; }
			private int counter;

			public MessageLayer()
			{
				IsAdded = false;
				HaveBeenDisposed = false;
				counter = 0;
			}

			protected override void OnAdded()
			{
				IsAdded = true;
			}

			protected override void OnRemoved()
			{
				IsAdded = false;
			}

			protected override void OnDispose()
			{
				HaveBeenDisposed = true;
			}

			protected override void OnUpdating()
			{
				counter++;
			}

			protected override void OnUpdated()
			{
				if(counter % 30 == 0)
				{
					AddObject(new MessageObject() { Position = new Vector2DF(200, 0) });
				}
			}

			protected override void OnDrawAdditionally()
			{
				DrawCircleAdditionally(new Vector2DF(100, 100), 100, 10, new Color(255, 0, 0), 12, 0, null, AlphaBlendMode.Blend, 2);
			}
		}

		class MessageScene : Scene
		{
			public int Phase { get; private set; }
			public bool HaveBeenDisposed { get; set; }

			public MessageScene()
			{
				Phase = 0;
				HaveBeenDisposed = false;
			}

			protected override void OnRegistered()
			{
				Assert.IsTrue(Phase == 0 || Phase == 6);
				Phase = 1;
			}

			protected override void OnStartUpdating()
			{
				Assert.AreEqual(1, Phase);
				Phase = 2;
			}

			protected override void OnTransitionFinished()
			{
				Assert.AreEqual(2, Phase);
				Phase = 3;
			}

			protected override void OnTransitionBegin()
			{
				Assert.AreEqual(3, Phase);
				Phase = 4;
			}

			protected override void OnStopUpdating()
			{
				Assert.AreEqual(4, Phase);
				Phase = 5;
			}

			protected override void OnUnregistered()
			{
				Assert.AreEqual(5, Phase);
				Phase = 6;
			}

			protected override void OnDispose()
			{
				HaveBeenDisposed = true;
			}
		}

		private MessageObject obj;
		private MessageLayer layer;
		private MessageScene scene;

		public LifeCycle() : base(120)
		{
		}

		protected override void OnStart()
		{
			scene = new MessageScene();
			layer = new MessageLayer();
			obj = new MessageObject();

			Assert.AreEqual(0, scene.Phase);
			Assert.AreEqual(false, scene.HaveBeenDisposed);
			Assert.AreEqual(false, layer.IsAdded);
			Assert.AreEqual(false, layer.HaveBeenDisposed);
			Assert.AreEqual(false, obj.IsAdded);
			Assert.AreEqual(false, obj.HaveBeenDisposed);

			layer.AddObject(obj);
			Assert.AreEqual(true, obj.IsAdded);
			layer.RemoveObject(obj);
			Assert.AreEqual(false, obj.IsAdded);
			layer.AddObject(obj);

			scene.AddLayer(layer);
			Assert.AreEqual(true, layer.IsAdded);
			scene.RemoveLayer(layer);
			Assert.AreEqual(false, layer.IsAdded);
			scene.AddLayer(layer);

			Engine.ChangeSceneWithTransition(scene, new TransitionFade(0.3f, 0.3f));
		}

		protected override void OnUpdating()
		{
			if(Time == 65)
			{
				Engine.ChangeSceneWithTransition(null, new TransitionFade(0.3f, 0.3f));
			}
		}

		protected override void OnFinish()
		{
			scene.Dispose();
			Assert.AreEqual(6, scene.Phase);
			Assert.AreEqual(true, scene.HaveBeenDisposed);
			Assert.AreEqual(true, layer.HaveBeenDisposed);
			Assert.AreEqual(true, obj.HaveBeenDisposed);
		}
	}
}
