using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using asd;

namespace unitTest_Engine_cs.ObjectSystem2D.LifeCycle
{
	class AddChildOnCtor : EngineTest
	{
		class MyObject : TextureObject2D
		{
			public MyObject()
			{
				Texture = Engine.Graphics.CreateTexture2D(CloudTexturePath);

				var child = new TextureObject2D()
				{
					Texture = Engine.Graphics.CreateTexture2D(CloudTexturePath),
					Position = new Vector2DF(20, 20)
				};
				AddDrawnChild(child, ChildManagementMode.RegistrationToLayer, ChildTransformingMode.Position, ChildDrawingMode.Nothing);
			}
		}

		public AddChildOnCtor()
			: base(60)
		{
		}

		protected override void OnStart()
		{
			var obj = new MyObject();
			Engine.AddObject2D(obj);
		}
	}
}
