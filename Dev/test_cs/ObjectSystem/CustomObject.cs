using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ace;

namespace test_cs.ObjectSystem
{
	class CustomObject : ISample
	{
		class HogeObject : TextureObject2D
		{
			protected override void OnStart()
			{
				Texture = Engine.Graphics.CreateTexture2D("Data/Texture/Cloud1.png");
			}

			protected override void OnUpdate()
			{
				Position += new Vector2DF( 1, 1 );
				Angle += 2;
			}
		}

		public void Run()
		{
			Engine.Initialize("CustomObject", 640, 480, false);

			var scene = new Scene();
			var layer = new Layer2D();
			var obj = new HogeObject();

			Engine.ChangeScene(scene);
			scene.AddLayer(layer);
			layer.AddObject(obj);

			while( Engine.DoEvents() )
			{
				Engine.Update();
			}

			Engine.Terminate();
		}
	}
}
