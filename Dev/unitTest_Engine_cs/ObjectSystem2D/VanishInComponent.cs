using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace unitTest_Engine_cs.ObjectSystem2D
{
	class VanishInComponent : EngineTest
	{
		class VanishingComponent : asd.Object2DComponent
		{
			int time = 0;

			asd.Object2D vanishedObject = null;
			int etime = 0;

			public VanishingComponent(asd.Object2D vanishedObject, int etime)
			{
				this.vanishedObject = vanishedObject;
				this.etime = etime;
			}

			protected override void OnUpdate()
			{
				if (time == etime)
				{
					vanishedObject.Dispose();
				}

				time++;
			}
		}

		asd.Layer2D layer = null;
		asd.Object2D obj = null;

		public VanishInComponent()
			: base(40)
		{
		}

		protected override void OnStart()
		{
			var scene = new asd.Scene();
			layer = new asd.Layer2D();
			obj = new asd.TextureObject2D();

			asd.Engine.ChangeScene(scene);
			scene.AddLayer(layer);
			layer.AddObject(obj);
		}

		protected override void OnUpdating()
		{
			if(Time % 2 == 0)
			{
				var temp = obj;
				obj = new asd.TextureObject2D();
				layer.AddObject(obj);
				obj.AddComponent(new VanishingComponent(temp, 3), "v");
			}
		}
	}
}
