using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using asd;

namespace unitTest_Engine_cs.ObjectSystem2D
{
	class ObjectsMembers : EngineTest
	{
		public ObjectsMembers()
			: base( 30 )
		{
		}

		protected override void OnStart()
		{
			var scene = new Scene();
			var layer = new Layer2D();
			var obj = new TextureObject2D();

			Engine.ChangeScene( scene );
			scene.AddLayer( layer );
			layer.AddObject( obj );

			AssertMembers( obj );
		}

		private void AssertMembers( TextureObject2D obj )
		{
			obj.AlphaBlend = AlphaBlendMode.Add;
			Assert.AreEqual( AlphaBlendMode.Add, obj.AlphaBlend );

			obj.Angle = 10;
			Assert.AreEqual( 10, obj.Angle );

			obj.CenterPosition = new Vector2DF( 128, 128 );
			Assert.AreEqual( new Vector2DF( 128, 128 ), obj.CenterPosition );

			obj.Color = new Color() { R = 10, G = 10, B = 10, A = 200 };
			Assert.AreEqual( new Color() { R = 10, G = 10, B = 10, A = 200 }, obj.Color );

			obj.DrawingPriority = 1;
			Assert.AreEqual( 1, obj.DrawingPriority );

			obj.Position = new Vector2DF( 100, 200 );
			Assert.AreEqual( new Vector2DF( 100, 200 ), obj.Position );

			obj.Scale = new Vector2DF( 2, 1 );
			Assert.AreEqual( new Vector2DF( 2, 1 ), obj.Scale );

			obj.TurnLR = true;
			Assert.AreEqual( true, obj.TurnLR );

			obj.TurnUL = true;
			Assert.AreEqual( true, obj.TurnUL );

			obj.Texture = Engine.Graphics.CreateTexture2D( CloudTexturePath );
			Assert.AreNotEqual( null, obj.Texture );
		}
	}
}
