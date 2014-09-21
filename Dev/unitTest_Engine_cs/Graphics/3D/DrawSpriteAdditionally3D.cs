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
			new ace.Vector3DF(-1, 0, -1),
			new ace.Vector3DF(1, 0, -1),
			new ace.Vector3DF(1, 0, 1),
			new ace.Vector3DF(-1, 0, 1),
			new ace.Color(255, 0, 0, 255),
			new ace.Color(0, 255, 0, 255),
			new ace.Color(0, 0, 255, 255),
			new ace.Color(255, 255, 255, 255),
			new ace.Vector2DF(0, 0),
			new ace.Vector2DF(0, 0),
			new ace.Vector2DF(0, 0),
			new ace.Vector2DF(0, 0),
			null,
			ace.AlphaBlendMode.Blend
			);
		}
	}
}
