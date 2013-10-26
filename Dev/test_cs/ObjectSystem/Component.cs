using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ace;

namespace test_cs.ObjectSystem
{
	class MyComponent : ace.Component
	{
		public MyComponent( Object2DBase owner )
		{
			this.owner = owner;
			time = 0;
		}

		protected override void OnUpdate()
		{
			if( time % 30 == 0 )
			{
				owner.Angle += 30;
			}
			++time;
		}

		private Object2DBase owner { get; set; }
		private int time { get; set; }
	}

	class Component : ISample
	{
		public void Run()
		{
			Engine.Initialize( "Ace Component", 640, 480, false );

			var scene = new Scene();
			var layer = new Layer2D();
			var obj = new TextureObject2D()
			{
				Position = new Vector2DF( 320, 240 ),
				Texture = Engine.Graphics.CreateTexture2D( "Data/Texture/Cloud1.png" ),
			};

			layer.AddObject( obj );
			scene.AddLayer( layer );
			Engine.ChangeScene( scene );
			obj.AddComponent( new MyComponent( obj ), "rotation" );

			while( Engine.DoEvents() )
			{
				Engine.Update();
			}

			Engine.Terminate();
		}
	}
}
