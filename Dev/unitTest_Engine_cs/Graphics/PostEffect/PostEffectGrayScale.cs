using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace unitTest_Engine_cs.Graphics.PostEffect
{
	class PostEffectGrayScale : EngineTest
	{
		public PostEffectGrayScale()
			: base(10)
		{
		}

		protected override void OnStart()
		{
			var scene = new ace.Scene();
			var layer = new ace.Layer2D();
			var obj = new ace.TextureObject2D();
			scene.AddLayer(layer);
			layer.AddObject(obj);
			ace.Engine.ChangeScene(scene);

			var g = ace.Engine.Graphics;
			var texture = g.CreateTexture2D("Data/Texture/Sample1.png");
			obj.Texture = texture;
			obj.Scale = new ace.Vector2DF(1, 1);

			var pe = new ace.PostEffectGrayScale();
			layer.AddPostEffect(pe);
		}
	}
}
