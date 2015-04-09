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
			: base(20)
		{
		}

		protected override void OnStart()
		{
			ace.SoundSource se1 = ace.Engine.Sound.CreateSoundSource("Data/Sound/metal28.wav", true);
			ace.SoundSource bgm1 = ace.Engine.Sound.CreateSoundSource("Data/Sound/bgm1.ogg", false);

			int id_se1 = ace.Engine.Sound.Play(se1);
			int id_bgm1 = ace.Engine.Sound.Play(bgm1);
		}
	}
}
