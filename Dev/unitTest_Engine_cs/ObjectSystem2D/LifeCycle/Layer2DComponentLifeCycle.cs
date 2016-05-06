using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using asd;

namespace unitTest_Engine_cs.ObjectSystem2D.LifeCycle
{
	class Layer2DComponentLifeCycle : EngineTest
	{
		private Scene scene;
		private Layer2D layer;
		private MyComponent component;

		private class MyComponent : Layer2DComponent
		{
			public bool IsAdded { get; set; }
			public int Count { get; set; }

			protected override void OnAdded()
			{
				IsAdded = true;
			}

			protected override void OnRemoved()
			{
				IsAdded = false;
			}

			protected override void OnUpdating()
			{
				Count++;
			}

			protected override void OnUpdated()
			{
				if(Count == 30)
				{
					Owner.AddObject(new TextureObject2D()
					{
						Texture = Engine.Graphics.CreateTexture2D(CloudTexturePath),
						Position = new Random().Next() % 8 * new Vector2DF(30, 30)
					});
				}
			}
		}

		public Layer2DComponentLifeCycle() : base(60)
		{
		}

		protected override void OnStart()
		{
			scene = new Scene();
			layer = new Layer2D();
			component = new MyComponent();

			layer.AddComponent(component, "LifeCycle");

			Assert.AreEqual(false, component.IsAdded);
			scene.AddLayer(layer);

			Engine.ChangeScene(scene);
		}

		protected override void OnUpdating()
		{
			if(Time == 10)
			{
				Assert.AreEqual(true, component.IsAdded);
				scene.RemoveLayer(layer);
			}
			if(Time == 20)
			{
				Assert.AreEqual(false, component.IsAdded);
				scene.AddLayer(layer);
			}
		}
	}
}
