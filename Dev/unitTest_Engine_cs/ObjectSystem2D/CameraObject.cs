using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ace;

namespace unitTest_Engine_cs.ObjectSystem2D
{
	class CameraObject : EngineTest
	{
		private CameraObject2D camera;

		public CameraObject()
			: base(60)
		{
		}

		protected override void OnStart()
		{
			var scene = new Scene();
			var layer = new Layer2D();
			var obj = new TextureObject2D();
			camera = new CameraObject2D();

			obj.Texture = Engine.Graphics.CreateTexture2D("Data/Texture/Sample1.png");
			camera.Src = new RectI(100, 100, 312, 312);
			camera.Dst = new RectI(10, 10, 200, 160);

			layer.AddObject(obj);
			layer.AddObject(camera);
			scene.AddLayer(layer);
			Engine.ChangeScene(scene);
		}

		protected override void OnUpdating()
		{
			var dst = camera.Dst;
			dst.X += 3;
			camera.Dst = dst;
		}
	}
}
