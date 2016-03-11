using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using asd;

namespace unitTest_Engine_cs.BugFix
{
	class ModifyObjectHavingDisposedChild : EngineTest
	{
		TextureObject2D parent, child;

		public ModifyObjectHavingDisposedChild() : base(30)
		{
		}

		protected override void OnStart()
		{
			Texture2D texture = Engine.Graphics.CreateTexture2D(CloudTexturePath);
			parent = new TextureObject2D()
			{
				Texture = texture,
			};
			child = new TextureObject2D()
			{
				Texture = texture,
				Position = new Vector2DF(0, 40),
				Color = new Color(255, 0, 0, 255),
			};

			parent.AddChild(child, ChildManagementMode.Nothing, ChildTransformingMode.Position);

			Engine.AddObject2D(parent);
			Engine.AddObject2D(child);
		}

		protected override void OnUpdated()
		{
			if(Time == 20)
			{
				child.Dispose();
			}
			if(Time == 25)
			{
				parent.Position += new Vector2DF(10, 0);
			}
		}
	}
}
