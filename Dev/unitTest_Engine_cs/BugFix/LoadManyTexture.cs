using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace unitTest_Engine_cs.BugFix
{
	class LoadManyTexture : EngineTest
	{
		public LoadManyTexture()
			: base(60)
		{
		}

		protected override void OnStart()
		{
			var tex1 = asd.Engine.Graphics.CreateTexture2D("Data/Texture/Cloud1.png");
			var tex2 = asd.Engine.Graphics.CreateTexture2D("Data/Texture/Sample1.png");
			var tex3 = asd.Engine.Graphics.CreateTexture2D("Data/Texture/PainRpg.png");
			var tex4 = asd.Engine.Graphics.CreateTexture2D("Data/Texture/PainRpgCopy.png");

			AddTexture(new asd.Vector2DF(0, 0), tex1);
			AddTexture(new asd.Vector2DF(150, 0), tex2);
			AddTexture(new asd.Vector2DF(300, 0), tex3);
			AddTexture(new asd.Vector2DF(450, 0), tex4);
		}

		private void AddTexture(asd.Vector2DF position, asd.Texture2D texture)
		{
			var obj = new asd.TextureObject2D()
			{
				Position = position,
				Texture = texture,
			};
			asd.Engine.AddObject2D(obj);
		}
	}
}
