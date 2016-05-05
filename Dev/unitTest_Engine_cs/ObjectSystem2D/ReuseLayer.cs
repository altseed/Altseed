using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using asd;

namespace unitTest_Engine_cs.ObjectSystem2D
{
	class ReuseLayer : EngineTest
	{
		class MyScene : Scene
		{
			protected override void OnUpdated()
			{
				var layer = Layers.First();
				var newScene = new Scene();
				RemoveLayer(layer);
				Engine.ChangeScene(newScene, false);
				newScene.AddLayer(layer);
			}
		}

		public ReuseLayer() : base(60)
		{
		}

		protected override void OnStart()
		{
			var scene = new MyScene();
			var layer = new Layer2D();
			var obj = new TextureObject2D
			{
				Texture = Engine.Graphics.CreateTexture2D(CloudTexturePath),
			};

			layer.AddObject(obj);
			scene.AddLayer(layer);
			Engine.ChangeScene(scene);
		}
	}
}
