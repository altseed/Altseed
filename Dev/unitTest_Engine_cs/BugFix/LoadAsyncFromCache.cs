using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace unitTest_Engine_cs.BugFix
{
	class LoadAsyncFromCache : EngineTest
	{
		public LoadAsyncFromCache()
			: base(60)
		{
		}

		protected override void OnUpdated()
		{
			if (Time == 0)
			{
				var file1 = asd.Engine.File.CreateStaticFileAsync("Data/Texture/Cloud1.png");
			}
			if (Time == 30)
			{
				var file2 = asd.Engine.File.CreateStaticFileAsync("Data/Texture/Cloud1.png");
			}
		}
	}
}
