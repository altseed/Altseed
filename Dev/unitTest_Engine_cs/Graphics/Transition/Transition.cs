using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace unitTest_Engine_cs.Graphics.Transition
{
	class Transition : EngineTest
	{
		public Transition()
			: base(150)
		{
		}

		protected override void OnStart()
		{
		}

		protected override void OnUpdating()
		{
			if(Time == 9)
			{
				var texture = asd.Engine.Graphics.CreateTexture2D("Data/Texture/Sample1.png");

				var obj = new asd.TextureObject2D();
				obj.Texture = texture;
				var scene = new asd.Scene();
				var layer = new asd.Layer2D();
				layer.AddObject(obj);
				scene.AddLayer(layer);

				var transition = new asd.TransitionFade(60.0f / 60.0f, 60.0f / 60.0f);
				asd.Engine.ChangeSceneWithTransition(scene, transition);
			}
		}
	}
}
