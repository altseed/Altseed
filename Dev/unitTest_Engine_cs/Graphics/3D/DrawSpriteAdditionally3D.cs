using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace unitTest_Engine_cs.Graphics._3D
{
	class DrawSpriteAdditionally3D : EngineGraphics3DTest
	{
		public DrawSpriteAdditionally3D()
			: base(10)
		{
		}

		protected override void OnStart()
		{
			base.OnStart();
		}

		protected override void OnUpdating()
		{
			base.OnUpdating();

			Layer3D.DrawSpriteAdditionally(
			new asd.Vector3DF(-1, 0, -1),
			new asd.Vector3DF(1, 0, -1),
			new asd.Vector3DF(1, 0, 1),
			new asd.Vector3DF(-1, 0, 1),
			new asd.Color(255, 0, 0, 255),
			new asd.Color(0, 255, 0, 255),
			new asd.Color(0, 0, 255, 255),
			new asd.Color(255, 255, 255, 255),
			new asd.Vector2DF(0, 0),
			new asd.Vector2DF(0, 0),
			new asd.Vector2DF(0, 0),
			new asd.Vector2DF(0, 0),
			null,
			asd.AlphaBlendMode.Blend,
			false,
			true
			);
		}
	}
}
