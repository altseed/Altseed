using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using asd;

namespace unitTest_Engine_cs.ObjectSystem2D
{
	class ThrowIfDisposed : EngineTest
	{
		public ThrowIfDisposed()
			: base(30)
		{
		}

		protected override void OnStart()
		{
			var scene = new Scene();
			var layer = new Layer2D();
			var obj = new TextureObject2D();

			Engine.ChangeScene(scene);
			scene.AddLayer(layer);
			layer.AddObject(obj);

			obj.Dispose();
			Assert.Throws<ObjectDisposedException>(() =>
			{
				obj.DrawingPriority = 2;
			});

			layer.Dispose();
			Assert.Throws<ObjectDisposedException>(() =>
			{
				layer.AddPostEffect(new PostEffectGaussianBlur());
			});

			scene.Dispose();
			Assert.Throws<ObjectDisposedException>(() =>
			{
				scene.AddLayer(new Layer3D());
			});
		}
	}
}
