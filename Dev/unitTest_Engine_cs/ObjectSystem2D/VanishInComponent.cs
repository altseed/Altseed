﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace unitTest_Engine_cs.ObjectSystem2D
{
	class VanishInComponent : EngineTest
	{
		ace.Layer2D layer = null;
		ace.Object2D obj = null;

		public VanishInComponent()
			: base(40)
		{
		}

		protected override void OnStart()
		{
			var scene = new ace.Scene();
			layer = new ace.Layer2D();
			obj = new ace.TextureObject2D();

			ace.Engine.ChangeScene(scene);
			scene.AddLayer(layer);
			layer.AddObject(obj);
		}

		protected override void OnUpdating()
		{
			if(Time % 2 == 0)
			{
				var temp = obj;
				obj = new ace.TextureObject2D();
				layer.AddObject(obj);
				obj.AddComponent(new VanishingComponent(temp, 3), "v");
			}
		}

		class VanishingComponent : ace.Object2DComponent
		{
			int time = 0;

			ace.Object2D vanishedObject = null;
			int etime = 0;

			public VanishingComponent(ace.Object2D vanishedObject, int etime)
			{
				this.vanishedObject = vanishedObject;
				this.etime = etime;
			}

			protected override void OnUpdate()
			{
				if(time == etime)
				{
					vanishedObject.Vanish();
				}

				time++;
			}
		}
	}
}
