using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ace;

namespace unitTest_Engine_cs.ObjectSystem2D
{
	class VanishInComponent : EngineTest
	{
		class MyComponent : Object2DComponent
		{
			private TextureObject2D vanish;

			public MyComponent(TextureObject2D vanish)
			{
				this.vanish = vanish;
			}

			protected override void OnUpdate()
			{
				vanish.Vanish();
			}
		}

		private TextureObject2D obj2;

		public VanishInComponent()
			: base(60)
		{
		}

		protected override void OnStart()
		{
			var scene = new ace.Scene();
			var layer = new ace.Layer2D();
			var obj = new ace.TextureObject2D();
			obj2 = new ace.TextureObject2D();

			ace.Engine.ChangeScene(scene);
			scene.AddLayer(layer);
			layer.AddObject(obj);

			var texture = Engine.Graphics.CreateTexture2D("Data/Texture/Cloud1.png");
			obj.Texture = texture;
			obj2.Texture = texture;

			obj.AddComponent(new MyComponent(obj2), "Hoge");
		}
	}
}
