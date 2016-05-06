using asd;

namespace unitTest_Engine_cs.ObjectSystem2D.LifeCycle
{
	class RemoveLayerAndDisposeScene : EngineTest
	{
		public RemoveLayerAndDisposeScene() : base(60)
		{
		}

		private Scene scene;
		private Layer2D layer;

		protected override void OnStart()
		{
			scene = new Scene();
			layer = new Layer2D();
			var obj = new TextureObject2D()
			{
				Texture = Engine.Graphics.CreateTexture2D(CloudTexturePath)
			};

			layer.AddObject(obj);
			scene.AddLayer(layer);
			Engine.ChangeScene(scene);
		}

		protected override void OnUpdated()
		{
			if (Time == 30)
			{
				scene.RemoveLayer(layer);
				Engine.ChangeScene(new Scene());
			}
			if(Time == 31)
			{
				Assert.AreEqual(true, layer.IsAlive);
			}
		}
	}
}
