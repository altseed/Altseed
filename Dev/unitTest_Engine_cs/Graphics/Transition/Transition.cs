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
			: base(20)
		{
		}

		protected override void OnStart()
		{
		}

		protected override void OnUpdating()
		{
			if (Time % 9 == 0)
		{
			var texture = ace.Engine.Graphics.CreateTexture2D("Data/Texture/Sample1.png");

			var obj = new ace.TextureObject2D();
			obj.Texture = texture;
			var scene = new ace.Scene();
			var layer = new ace.Layer2D();
			layer.AddObject(obj);
			scene.AddLayer(layer);

			var transition = new ace.TransitionFade(3, 3);
			ace.Engine.ChangeSceneWithTransition(scene, transition);
		}
		}
	}
}
