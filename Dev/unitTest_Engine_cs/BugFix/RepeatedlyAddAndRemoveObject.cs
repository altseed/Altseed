using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using asd;

namespace unitTest_Engine_cs.BugFix
{
	class MyObject_RepeatedlyAddAndRemoveObject : TextureObject2D
	{
		protected override void OnAdded()
		{
			Console.WriteLine("OnAdded");
			Assert.AreNotEqual(null, Layer);
		}

		protected override void OnRemoved()
		{
			Console.WriteLine("OnRemoved");
		}
	}

	class RepeatedlyAddAndRemoveObject : EngineTest
	{
		public RepeatedlyAddAndRemoveObject()
			: base(60)
		{
		}

		protected override void OnStart()
		{
			var scene = new Scene();
			var layer = new Layer2D();
			var layer2 = new Layer2D();
			var obj = new MyObject_RepeatedlyAddAndRemoveObject();

			Engine.ChangeScene(scene);
			scene.AddLayer(layer);

			for (int i = 0; i < 4; i++)
			{
				layer.AddObject(obj);
				layer.RemoveObject(obj);
			}
			for(int i = 0; i < 4; i++)
			{
				layer2.AddObject(obj);
				layer2.RemoveObject(obj);
			}
		}
	}
}
