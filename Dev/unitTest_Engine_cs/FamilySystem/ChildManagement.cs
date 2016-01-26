using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using asd;

namespace unitTest_Engine_cs.FamilySystem
{
	class ChildManagement : EngineTest
	{
		class SpinObject : asd.TextureObject2D
		{
			protected override void OnUpdate()
			{
				Angle += 2;
			}
		}

		class Family
		{
			SpinObject parent, child, childNotAdded;
			Vector2DF position;
			ChildManagementMode mode;

			public SpinObject Parent
			{
				get { return parent; }
			}

			public Family(Vector2DF position, ChildManagementMode mode)
			{
				this.position = position;
				this.mode = mode;
			}

			public void Initialize()
			{
				parent = new SpinObject();
				child = new SpinObject();
				childNotAdded = new SpinObject();

				var texture = Engine.Graphics.CreateTexture2D(CloudTexturePath);
				parent.Texture = texture;
				parent.CenterPosition = texture.Size.To2DF() / 2;
				parent.Scale = new Vector2DF(0.5f, 0.5f);
				parent.Position = position;

				child.Texture = texture;
				child.CenterPosition = texture.Size.To2DF() / 2;
				child.Scale = new Vector2DF(0.5f, 0.5f);
				child.Position = position + new Vector2DF(0, 100);

				childNotAdded.Texture = texture;
				childNotAdded.CenterPosition = texture.Size.To2DF() / 2;
				childNotAdded.Scale = new Vector2DF(0.5f, 0.5f);
				childNotAdded.Position = position + new Vector2DF(0, 200);

				parent.AddChild(child, mode, ChildTransformingMode.Nothing);
				parent.AddChild(childNotAdded, mode, ChildTransformingMode.Nothing);

				Engine.AddObject2D(parent);
				if((mode & ChildManagementMode.RegistrationToLayer) == 0)
				{
					Engine.AddObject2D(child);
				}
			}
		}

		public ChildManagement() : base(60)
		{
		}

		private Family nothing, isDrawnChanged, isUpdateChanged, registrationToLayerChanged, disposed;

		protected override void OnStart()
		{
			nothing = new Family(new Vector2DF(70, 70), ChildManagementMode.Nothing);
			isDrawnChanged = new Family(new Vector2DF(190, 70), ChildManagementMode.IsDrawn);
			isUpdateChanged = new Family(new Vector2DF(310, 70), ChildManagementMode.IsUpdated);
			registrationToLayerChanged = new Family(new Vector2DF(430, 70), ChildManagementMode.RegistrationToLayer);
			disposed = new Family(new Vector2DF(550, 70), ChildManagementMode.Vanishment);

			nothing.Initialize();
			isDrawnChanged.Initialize();
			isUpdateChanged.Initialize();
			registrationToLayerChanged.Initialize();
			disposed.Initialize();
		}

		protected override void OnUpdating()
		{
			if(Time == 30)
			{
				isDrawnChanged.Parent.IsDrawn = false;
				isUpdateChanged.Parent.IsUpdated = false;
				Engine.RemoveObject2D(registrationToLayerChanged.Parent);
				disposed.Parent.Dispose();
			}
		}
	}
}
