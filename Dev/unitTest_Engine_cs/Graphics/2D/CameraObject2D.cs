using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace unitTest_Engine_cs.ObjectSystem2D
{
	class CameraObject2D : EngineTest
	{
		private ace.CameraObject2D camera;

		public CameraObject2D()
			: base(60)
		{
		}

		protected override void OnStart()
		{
			var scene = new ace.Scene();
			var layer = new ace.Layer2D();
			var obj = new ace.TextureObject2D();
			camera = new ace.CameraObject2D();

			obj.Texture = ace.Engine.Graphics.CreateTexture2D("Data/Texture/Sample1.png");
			camera.Src = new ace.RectI(100, 100, 312, 312);
			camera.Dst = new ace.RectI(10, 10, 200, 200);

			layer.AddObject(obj);
			layer.AddObject(camera);
			scene.AddLayer(layer);
			ace.Engine.ChangeScene(scene);
		}

		protected override void OnUpdating()
		{
			var dst = camera.Dst;
			dst.X += 3;
			camera.Dst = dst;
		}
	}
}
