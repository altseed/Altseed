using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using asd;

namespace unitTest_Engine_cs.ObjectSystem2D
{
	class CustomObject : EngineTest
	{
		class HogeObject : TextureObject2D
		{
			protected override void OnAdded()
			{
				Texture = Engine.Graphics.CreateTexture2D( CloudTexturePath );
			}

			protected override void OnUpdate()
			{
				Position += new Vector2DF( 3, 3 );
				Angle += 2;
			}
		}

		public CustomObject()
			: base(40)
		{
		}

		protected override void OnStart()
		{
			var scene = new Scene();
			var layer = new Layer2D();
			var obj = new HogeObject();

			Engine.ChangeScene( scene );
			scene.AddLayer( layer );
			layer.AddObject( obj );
		}
	}
}
