using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using asd;

namespace unitTest_Engine_cs.Graphics._2D
{
	class PlayEffectBeforeRegister : EngineTest
	{
		public PlayEffectBeforeRegister() : base(60)
		{
		}

		private asd.EffectObject2D obj;

		protected override void OnStart()
		{
			obj = new asd.EffectObject2D()
			{
				Effect = Engine.Graphics.CreateEffect("Data/Effect/Laser01.efk"),
				Position = new Vector2DF(320, 240),
				Scale = new Vector2DF(10, 10),
				EffectRotation = 90
			};

			Engine.AddObject2D(obj);
			obj.Play();
		}
	}
}
