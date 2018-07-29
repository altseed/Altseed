using asd;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace unitTest_Engine_cs.BugFix
{
	class AsyncLoadAndSound : EngineTest
	{
		private static readonly string[] Files = new string[]
		{
			"Data/Texture/PainRpg.png",
			"Data/Texture/PainRpgCopy.png",
			"Data/Texture/Sample1.png",
			"Data/Texture/SamplePackage.pack",
			"Data/Texture/Cloud1.png",
			"Data/Effect/Laser01.efk"
		};

		private List<StaticFile> files = new List<StaticFile>();
		private SoundSource sound;
		private int index;

		public AsyncLoadAndSound()
			: base(120)
		{
		}

		protected override void OnStart()
		{
			index = 0;
			sound = Engine.Sound.CreateSoundSource("Data/Sound/bgm1.ogg", true);
			Engine.Sound.Play(sound);

			foreach (var item in Files)
			{
				files.Add(Engine.File.CreateStaticFileAsync(item));
			}
		}
	}
}
