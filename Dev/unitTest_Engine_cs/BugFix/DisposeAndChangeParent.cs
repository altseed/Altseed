using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using asd;

namespace unitTest_Engine_cs.BugFix
{
	class DisposeAndChangeParent : EngineTest
	{
		TextureObject2D child, parent1, parent2, parent3;

		public DisposeAndChangeParent()
			: base(120)
		{
		}

		protected override void OnStart()
		{
			Texture2D texture = Engine.Graphics.CreateTexture2D(CloudTexturePath);
			parent1 = new TextureObject2D()
			{
				Texture = texture,
				Position = new Vector2DF(0, 40),
				Color = new Color(0, 255, 0),
			};
			parent2 = new TextureObject2D()
			{
				Texture = texture,
				Position = new Vector2DF(0, 80),
				Color = new Color(128, 128, 128),
			};
			parent3 = new TextureObject2D()
			{
				Texture = texture,
				Position = new Vector2DF(0, 120),
				Color = new Color(128, 0, 0),
			};
			child = new TextureObject2D()
			{
				Texture = texture,
				Position = new Vector2DF(450, 0),
			};

			parent1.AddChild(child, ChildManagementMode.Nothing, ChildTransformingMode.Position);

			Engine.AddObject2D(parent1);
			Engine.AddObject2D(parent2);
			Engine.AddObject2D(parent3);
			Engine.AddObject2D(child);
		}

		protected override void OnUpdating()
		{
			if(Time == 30)
			{
				parent1.Dispose();
				parent2.AddChild(child, ChildManagementMode.Nothing, ChildTransformingMode.Position);
			}
			if(Time == 60)
			{
				if(child.Parent != null)
				{
					child.Parent.RemoveChild(child);
				}
				parent3.AddChild(child, ChildManagementMode.Nothing, ChildTransformingMode.Position);
			}
			if(Time == 90)
			{
				if(child.Parent != null)
				{
					child.Parent.RemoveChild(child); 
				}
				parent2.AddChild(child, ChildManagementMode.Nothing, ChildTransformingMode.Position);
			}
		}
	}
}
