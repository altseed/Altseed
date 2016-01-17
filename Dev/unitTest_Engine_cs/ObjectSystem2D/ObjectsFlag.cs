using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using asd;

namespace unitTest_Engine_cs.ObjectSystem2D
{
	class ObjectsFlag : EngineTest
	{
		class MyObject : TextureObject2D
		{
			public MyObject( float x, float y )
			{
				Position = new Vector2DF( x, y );
			}

			protected override void OnAdded()
			{
				var g = Engine.Graphics.CreateTexture2D( CloudTexturePath );
				Texture = g;
				CenterPosition = new Vector2DF( Texture.Size.X / 2, Texture.Size.Y / 2 );
			}

			protected override void OnUpdate()
			{
				Angle += 2;
			}
		}

		public ObjectsFlag()
			: base(30)
		{
		}

		protected override void OnStart()
		{
			var scene = new Scene();
			var layer = new Layer2D();
			var obj = new MyObject( 100, 128 );
			var notDrawnObject = new MyObject( 300, 128 );
			var notUpdatedObject = new MyObject( 500, 128 );

			notDrawnObject.IsDrawn = false;
			notUpdatedObject.IsUpdated = false;

			layer.AddObject( obj );
			layer.AddObject( notDrawnObject );
			layer.AddObject( notUpdatedObject );
			scene.AddLayer( layer );
			Engine.ChangeScene( scene );
		}
	}
}
