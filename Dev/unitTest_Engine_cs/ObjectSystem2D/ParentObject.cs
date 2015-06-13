using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using asd;

namespace unitTest_Engine_cs.ObjectSystem2D
{
	class ParentObject : EngineTest
	{
		public ParentObject()
			: base( 60 )
		{
		}

		protected override void OnStart()
		{
			var scene = new Scene();
			var layer = new Layer2D();
			var child = new TextureObject2D();
			parent = new TextureObject2D();

			parent.AddChild( child, ChildMode.All );
			layer.AddObject( parent );
			layer.AddObject( child );
			scene.AddLayer( layer );
			Engine.ChangeScene( scene );

			var texture = Engine.Graphics.CreateTexture2D( CloudTexturePath );

			parent.Position = new Vector2DF( 320, 240 );
			parent.CenterPosition = new Vector2DF( 128, 128 );
			parent.Texture = texture;

			child.Position = new Vector2DF( 50, 50 );
			child.Texture = texture;
		}

		protected override void OnUpdating()
		{
			parent.Angle += 0.2f;
			parent.Scale = new Vector2DF() { X = parent.Scale.X + 0.0008f, Y = parent.Scale.Y + 0.0008f };
		}

		private TextureObject2D parent { get; set; }
	}
}
