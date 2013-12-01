using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ace;

namespace unitTest_Engine_cs.ObjectSystem
{
	class ObjectSimple : EngineTest
	{
		public ObjectSimple()
			: base( 120 )
		{
		}

		protected override void OnStart()
		{
			var scene = new Scene();
			var layer = new Layer2D();
			obj = new TextureObject2D()
			{
				Texture = Engine.Graphics.CreateTexture2D( "Data/Texture/Cloud1.png" ),
				Position = new Vector2DF() { X = 320, Y = 240 },
				Angle = 5,
				CenterPosition = new Vector2DF() { X = 128, Y = 128 },
			};

			layer.AddObject( obj );
			scene.AddLayer( layer );
			Engine.ChangeScene( scene );
		}

		protected override void OnUpdating()
		{
			obj.Scale += new Vector2DF( 0.0005f, 0.0005f );
		}

		protected override void OnFinish()
		{
			obj = null;
		}

		private TextureObject2D obj;
	}
}
