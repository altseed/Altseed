using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace unitTest_Engine_cs.Sound
{
	class Sound : EngineTest
	{
		public Sound()
			: base(80)
		{
		}

		protected override void OnStart()
		{
			asd.SoundSource se1 = asd.Engine.Sound.CreateSoundSource("Data/Sound/se1.wav", true);
			asd.SoundSource bgm1 = asd.Engine.Sound.CreateSoundSource("Data/Sound/bgm1.ogg", false);

			int id_se1 = asd.Engine.Sound.Play(se1);
			int id_bgm1 = asd.Engine.Sound.Play(bgm1);
		}
	}
}
