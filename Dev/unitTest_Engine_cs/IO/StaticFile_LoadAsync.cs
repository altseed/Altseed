using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace unitTest_Engine_cs.IO
{
	class StaticFile_LoadAsync : EngineTest
	{
		private List<byte> loadData;
		private asd.StaticFile staticFile;
		private bool isFinished = false;

		public StaticFile_LoadAsync()
			: base(60)
		{
		}

		protected override void OnStart()
		{
			using (FileStream fileStream = new FileStream("Data/Texture/PainRpg.png",
				FileMode.Open,
				FileAccess.Read))
			{
				byte[] buf = new byte[fileStream.Length];
				fileStream.Read(buf, 0, buf.Length);
				loadData = new List<byte>(buf);
			}

			staticFile = asd.Engine.File.CreateStaticFileAsync("Data/Texture/PainRpg.png");
		}

		protected override void OnUpdated()
		{
			if (!isFinished && staticFile.LoadState == asd.LoadState.Loaded)
			{
				var staticFileBytes = staticFile.Buffer;
				Assert.Equals(loadData.Count, staticFileBytes.Length);
				for (int i = 0; i < loadData.Count; i++)
				{
					byte raw = loadData[i];
					byte file = staticFileBytes[i];
					Assert.Equals(raw, file);
				}

				using (var file = File.Create("Data/Texture/PainRpgCopy.png"))
				{
					file.WriteAsync(staticFileBytes, 0, staticFileBytes.Length)
						.Wait();
				}

				isFinished = true;
			}
			else
			{
				Console.WriteLine("Loading...");
			}
		}
	}
}
