using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ace;

namespace test_cs.ObjectSystem
{
	class ParentObject : ISample
	{
		public void Run()
		{
			Engine.Initialize("ParentObject", 1280, 720, new EngineOption());

			var scene = new Scene();
			var layer = new Layer2D();
			var parent = new TextureObject2D();
			var child = new TextureObject2D();

			parent.AddChild( child, ChildMode.All );
			layer.AddObject( parent );
			layer.AddObject( child );
			scene.AddLayer( layer );
			Engine.ChangeScene( scene );

			var texture = Engine.Graphics.CreateTexture2D("Data/Texture/Cloud1.png");

			parent.Position = new Vector2DF() { X = 640, Y = 360 };
			parent.CenterPosition = new Vector2DF() { X = 128, Y = 128 };
			parent.Texture = texture;
			child.Position = new Vector2DF() { X = 50, Y = 50 };
			child.Texture = texture;

			while( Engine.DoEvents() )
			{
				parent.Angle += 0.1f;
				parent.Scale = new Vector2DF() { X = parent.Scale.X + 0.0005f, Y = parent.Scale.Y + 0.0005f };
				Engine.Update();
			}

			Engine.Terminate();
		}
	}
}
