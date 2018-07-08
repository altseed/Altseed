using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace unitTest_Engine_cs.BugFix
{
	class LoadAsyncFromCache : EngineTest
	{
		asd.StaticFile file1;
		float loadedTime = 0;

		public LoadAsyncFromCache()
			: base(60)
		{
		}

		protected override void OnUpdated()
		{
			if (Time == 0)
			{
				file1 = asd.Engine.File.CreateStaticFileAsync("Data/Texture/Cloud1.png");
			}

			if (file1 != null && file1.LoadState == asd.LoadState.Loaded)
			{
				file1 = null;
				loadedTime = Time;
				GC.Collect();
				Console.WriteLine("file1 loaded.");
			}

			if (Time == loadedTime + 5)
			{
				var file2 = asd.Engine.File.CreateStaticFileAsync("Data/Texture/Cloud1.png");
				Console.WriteLine("file2 loaded.");
			}
		}
	}
}
