using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ace;

namespace unitTest_Engine_cs.ObjectSystem
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
			var obj = new TextureObject2D()
			{
				Position = new Vector2DF( 320, 240 ),
				Texture = Engine.Graphics.CreateTexture2D( CloudTexturePath ),
			};

			Engine.ChangeScene( scene );
			scene.AddLayer( layer );
			layer.AddObject( obj );
			obj.AddComponent( new MyComponent(), "Rotation" );
		}
	}
}
