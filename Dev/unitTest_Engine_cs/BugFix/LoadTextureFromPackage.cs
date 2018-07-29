using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace unitTest_Engine_cs.BugFix
{
	class LoadTextureFromPackage : EngineTest
	{
		public LoadTextureFromPackage()
			: base(60)
		{
		}

		protected override void OnStart()
		{
			asd.Engine.File.AddRootPackage("Data/SamplePackage.pack");
			var texture = asd.Engine.Graphics.CreateTexture2D("Cloud1.png");
			var obj = new asd.TextureObject2D()
			{
				Texture = texture,
			};
			asd.Engine.AddObject2D(obj);
		}
	}
}
