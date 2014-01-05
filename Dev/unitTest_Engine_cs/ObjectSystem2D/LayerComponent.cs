using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ace;

namespace unitTest_Engine_cs.ObjectSystem2D
{
	class LayerComponent : EngineTest
	{
		class MyComponent : Layer2DComponent
		{
			protected override void OnUpdated()
			{
				Owner.IsDrawn = false;
			}
		}

		public LayerComponent()
			: base( 60 )
		{
		}

		protected override void OnStart()
		{
			var scene = new Scene();
			var layer1 = CreateSampleLayer( 240 );
			var layer2 = CreateSampleLayer( 400 );

			scene.AddLayer( layer1.Item1 );
			scene.AddLayer( layer2.Item1 );
			Engine.ChangeScene(scene);

			layer2.Item2.Vanish();
		}

		private Tuple<Layer2D, MyComponent> CreateSampleLayer( float x )
		{
			var layer = new Layer2D();
			var obj = new TextureObject2D
			{
				Position = new Vector2DF( x, 240 ),
				Texture = Engine.Graphics.CreateTexture2D( CloudTexturePath ),
			};
			var component = new MyComponent();

			layer.AddObject( obj );
			layer.AddComponent( component, "Black" );

			return Tuple.Create( layer, component );
		}
	}
}
