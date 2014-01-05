using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ace;

namespace unitTest_Engine_cs.ObjectSystem2D
{
	class ObjectComponent : EngineTest
	{
		class MyComponent : Object2DComponent
		{
			protected override void OnUpdate()
			{
				Owner.Angle += 1;
			}
		}

		public ObjectComponent()
			: base( 60 )
		{
		}

		protected override void OnStart()
		{
			var scene = new Scene();
			var layer = new Layer2D();

			var obj1 = GetObject( 160 );
			var obj2 = GetObject( 480 );

			Engine.ChangeScene( scene );
			scene.AddLayer( layer );
			layer.AddObject( obj1.Item1 );
			layer.AddObject( obj2.Item1 );

			obj2.Item2.Vanish();
		}

		private Tuple<TextureObject2D, MyComponent> GetObject( float x )
		{
			var obj = new TextureObject2D()
			{
				Position = new Vector2DF( x, 240 ),
				Texture = Engine.Graphics.CreateTexture2D( CloudTexturePath )
			};

			var component = new MyComponent();

			obj.AddComponent( component, "Rotation" );

			return Tuple.Create( obj, component );
		}
	}
}
