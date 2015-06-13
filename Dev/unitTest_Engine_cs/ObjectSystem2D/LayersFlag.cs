using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using asd;

namespace unitTest_Engine_cs.ObjectSystem2D
{
	class LayersFlag : EngineTest
	{
		Scene scene;
		Layer2D layer;

		class MyObject : TextureObject2D
		{
			public MyObject( float x, float y )
			{
				Position = new Vector2DF( x, y );
			}

			protected override void OnStart()
			{
				Texture = Engine.Graphics.CreateTexture2D( CloudTexturePath );
				CenterPosition = new Vector2DF( Texture.Size.X / 2, Texture.Size.Y / 2 );
			}

			protected override void OnUpdate()
			{
				Angle += 2;
			}
		}

		public LayersFlag()
			: base(30)
		{
		}

		protected override void OnStart()
		{
			scene = new Scene();
			layer = new Layer2D();
			var notDrawnLayer = new Layer2D();
			var notUpdatedLayer = new Layer2D();
			var object1 = new MyObject( 100, 128 );
			var object2 = new MyObject( 300, 128 );
			var object3 = new MyObject( 500, 128 );

			notDrawnLayer.IsDrawn = false;
			notUpdatedLayer.IsUpdated = false;

			layer.AddObject( object1 );
			notDrawnLayer.AddObject( object2 );
			notUpdatedLayer.AddObject( object3 );
			scene.AddLayer( layer );
			scene.AddLayer( notDrawnLayer );
			scene.AddLayer( notUpdatedLayer );

			Engine.ChangeScene( scene );
		}

		protected override void OnUpdating()
		{
			// レイヤー削除テスト
			if (Time == ExitTime)
			{
				scene.RemoveLayer(layer);
			}
		}
	}
}
