using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ace;

namespace unitTest_Engine_cs.ObjectSystem2D
{
	class ComponentError0408 : EngineTest
	{
		class MyComponent : Object2DComponent
		{
			private Action<float> setter;
			private float time;

			public MyComponent(Action<float> setter)
			{
				this.setter = setter;
			}

			protected override void OnUpdate()
			{
				time += Engine.DeltaTime * 10;
				setter(time);
			}
		}

		class MyObject : TextureObject2D
		{
			TextObject2D child;
			public MyObject()
			{
				Position = new Vector2DF(320, 0);
				child = new TextObject2D()
				{
					Font = ace.Engine.Graphics.CreateFont("Data/Font/Nac0812.aff"),
					Text = "D",
				};
				AddChild(child, ChildMode.Position);
				AddComponent(new MyComponent(v => Position = new Vector2DF(Position.X, v)), "Hoge");
			}

			protected override void OnStart()
			{
				Layer.AddObject(child);
			}

			protected override void OnVanish()
			{
				child.Vanish();
			}
		}

		public ComponentError0408()
			: base(60)
		{
		}

		protected override void OnStart()
		{
			var scene = new Scene();
			var layer = new Layer2D();
			var obj = new MyObject();

			Engine.ChangeScene(scene);
			scene.AddLayer(layer);
			layer.AddObject(obj);
		}
	}
}
