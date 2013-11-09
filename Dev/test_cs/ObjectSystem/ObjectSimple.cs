using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ace;

namespace test_cs.ObjectSystem
{
	class ObjectSimple : ISample
	{
		public void Run()
		{
			Engine.Initialize("Ace Object", 640, 480, new EngineOption());

			var scene = new Scene();
			var layer = new Layer2D();
			var obj = new TextureObject2D()
			{
				Texture = Engine.Graphics.CreateTexture2D( "Data/Texture/Cloud1.png" ),
				Position = new Vector2DF() { X = 320, Y = 240 },
				Angle = 5,
				CenterPosition = new Vector2DF(){ X = 128, Y = 128 },
			};

			layer.AddObject( obj );
			scene.AddLayer( layer );
			Engine.ChangeScene(scene);

			var profiler = Engine.Profiler;
			while( Engine.DoEvents() )
			{
				obj.Scale += new Vector2DF( 0.0002f, 0.0002f );

				profiler.Start( 1 );
				Engine.Update();
				profiler.End( 1 );
			}

			Engine.Terminate();
		}
	}
}
