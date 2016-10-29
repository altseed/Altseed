using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using asd;

namespace unitTest_Engine_cs.BugFix
{
	class AddObjectThenAddChild : EngineTest
	{
		public AddObjectThenAddChild()
			: base(60)
		{
		}

		protected override void OnStart()
		{
			var layer = new Layer2D();
			var obj1 = new TextureObject2D();
			var obj2 = new TextureObject2D();

			layer.AddObject(obj1);
			obj1.AddChild(obj2, ChildManagementMode.Nothing, ChildTransformingMode.All);
			Assert.AreEqual(layer, obj2.Layer);
		}
	}
}
