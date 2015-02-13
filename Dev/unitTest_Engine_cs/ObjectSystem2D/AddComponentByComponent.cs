using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ace;

namespace unitTest_Engine_cs.ObjectSystem2D
{
	class AddComponentByComponent : EngineTest
	{
		class AddingObjectComponent : Object2DComponent
		{
			int count = 0;
			protected override void OnUpdate()
			{
				count++;
				if(count == 10)
				{
					Owner.AddComponent(new AddingObjectComponent(), "Add");
					Console.WriteLine("vanish object component.");
					Vanish();
				}
			}
		}

		class AddingLayerComponent : Layer2DComponent
		{
			int count = 0;
			protected override void OnUpdated()
			{
				count++;
				if(count == 10)
				{
					Owner.AddComponent(new AddingLayerComponent(), "Add");
					Console.WriteLine("vanish layer component.");
					Vanish();
				}
			}
		}

		class AddingSceneComponent : SceneComponent
		{
			int count = 0;
			protected override void OnUpdated()
			{
				count++;
				if(count == 10)
				{
					Owner.AddComponent(new AddingSceneComponent(), "Add");
					Console.WriteLine("vanish scene component.");
					Vanish();
				}
			}
		}

		public AddComponentByComponent()
			: base(60)
		{

		}

		protected override void OnStart()
		{
			var scene = new Scene();
			var layer = new Layer2D();
			var obj = new TextureObject2D();

			Engine.ChangeScene(scene);
			scene.AddLayer(layer);
			layer.AddObject(obj);

			scene.AddComponent(new AddingSceneComponent(), "Add");
			layer.AddComponent(new AddingLayerComponent(), "Add");
			obj.AddComponent(new AddingObjectComponent(), "Add");
		}
	}
}
