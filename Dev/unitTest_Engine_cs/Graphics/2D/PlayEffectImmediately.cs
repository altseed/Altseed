using asd;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace unitTest_Engine_cs.Graphics._2D
{
	class PlayEffectImmediately : EngineTest
	{
		public PlayEffectImmediately()
			: base(60)
		{
		}

		protected override void OnStart()
		{
			var effect = Engine.Graphics.CreateEffect("Data/Effect/Laser01.efk");
			var effectObj = new asd.EffectObject2D();

			effectObj.Position = new asd.Vector2DF(320, 240);
			effectObj.Scale = new asd.Vector2DF(20, 20);
			effectObj.EffectRotation = 90;
			effectObj.Effect = effect;

			Engine.AddObject2D(effectObj);
			effectObj.Play();
		}
	}
}
