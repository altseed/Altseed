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
		class ComponentAddingObject : Object2DComponent
		{
			int count = 0;
			protected override void OnUpdate()
			{
				count++;
				if(count == 10)
				{
					Owner.AddComponent(new ComponentAddingObject(), "Add");
					Console.WriteLine("vanish object component.");
					Vanish();
				}
			}
		}

		class ComponentAddingLayer : Layer2DComponent
		{
			int count = 0;
			protected override void OnUpdated()
			{
				count++;
				if(count == 10)
				{
					Owner.AddComponent(new ComponentAddingLayer(), "Add");
					Console.WriteLine("vanish layer component.");
					Vanish();
				}
			}
		}

		class ComponentAddingScene : SceneComponent
		{
			int count = 0;
			protected override void OnUpdated()
			{
				count++;
				if(count == 10)
				{
					Owner.AddComponent(new ComponentAddingScene(), "Add");
					Console.WriteLine("vanish scene component.");
					Vanish();
				}
			}
		}

		class ObjectAddingObject : TextureObject2D
		{
			int count = 0;
			protected override void OnUpdate()
			{
				count++;
				if(count == 10)
				{
					Layer.AddObject(new ObjectAddingObject());
					Console.WriteLine("vanish object.");
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
			var obj = new ObjectAddingObject();

			Engine.ChangeScene(scene);
			scene.AddLayer(layer);
			layer.AddObject(obj);

			scene.AddComponent(new ComponentAddingScene(), "Add");
			layer.AddComponent(new ComponentAddingLayer(), "Add");
			obj.AddComponent(new ComponentAddingObject(), "Add");
		}
	}
}
