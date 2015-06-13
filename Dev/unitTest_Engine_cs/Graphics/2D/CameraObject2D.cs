using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace unitTest_Engine_cs.ObjectSystem2D
{
	class CameraObject2D : EngineTest
	{
		private asd.CameraObject2D camera;

		public CameraObject2D()
			: base(60)
		{
		}

		protected override void OnStart()
		{
			var scene = new asd.Scene();
			var layer = new asd.Layer2D();
			var obj = new asd.TextureObject2D();
			camera = new asd.CameraObject2D();

			obj.Texture = asd.Engine.Graphics.CreateTexture2D("Data/Texture/Sample1.png");
			camera.Src = new asd.RectI(100, 100, 312, 312);
			camera.Dst = new asd.RectI(10, 10, 200, 200);

			layer.AddObject(obj);
			layer.AddObject(camera);
			scene.AddLayer(layer);
			asd.Engine.ChangeScene(scene);
		}

		protected override void OnUpdating()
		{
			var dst = camera.Dst;
			dst.X += 3;
			camera.Dst = dst;
		}
	}
}
