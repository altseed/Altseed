using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using asd;

namespace unitTest_Engine_cs.BugFix
{
	class DisposeBug : EngineTest
	{
		class ParentLayer : Layer2D
		{
			private Layer2D inner;

			public ParentLayer()
			{
				inner = new Layer2D();
			}

			protected override void OnAdded()
			{
				Scene.AddLayer(inner);
			}

			public void AddObjectToInner(Object2D obj)
			{
				inner.AddObject(obj);
			}

			protected override void OnDispose()
			{
				inner.Dispose();
			}
		}

		class DisposingObject : TextureObject2D
		{
			private ParentLayer layer;
			private int time;

			public DisposingObject(ParentLayer layer)
			{
				this.layer = layer;
				Texture = Engine.Graphics.CreateTexture2D(CloudTexturePath);
			}

			protected override void OnUpdate()
			{
				++time;
				if (time == 30)
				{
					layer.Dispose();
					GC.Collect();
				}
			}
		}

		public DisposeBug()
			: base(60)
		{
		}

		protected override void OnStart()
		{
			var scene = new Scene();
			var layer = new ParentLayer();
			var obj = new DisposingObject(layer);
			var obj1 = new TextureObject2D();
			var obj2 = new TextureObject2D();
			var obj3A = new TextureObject2D();
			var obj3B = new TextureObject2D();

			obj.AddChild(obj1, ChildManagementMode.RegistrationToLayer | ChildManagementMode.Disposal, ChildTransformingMode.Position);
			obj1.AddChild(obj2, ChildManagementMode.RegistrationToLayer | ChildManagementMode.Disposal, ChildTransformingMode.Position);
			obj2.AddChild(obj3A, ChildManagementMode.RegistrationToLayer | ChildManagementMode.Disposal, ChildTransformingMode.Position);
			obj2.AddChild(obj3B, ChildManagementMode.RegistrationToLayer | ChildManagementMode.Disposal, ChildTransformingMode.Position);

			scene.AddLayer(layer);
			layer.AddObject(obj);
			Engine.ChangeScene(scene);
		}
	}
}
