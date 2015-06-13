using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace sample_cs.Sound
{
	/// <summary>
	/// 音を鳴らすサンプル
	/// </summary>
	class Sound : ISample
	{
		public void Run()
		{
			// aceを初期化する。
			asd.Engine.Initialize("Sound", 640, 480, new asd.EngineOption());

			// ファイルを読み込む
			asd.SoundSource se1 = asd.Engine.Sound.CreateSoundSource("Data/Sound/se1.wav", true);
			asd.SoundSource bgm1 = asd.Engine.Sound.CreateSoundSource("Data/Sound/bgm1.ogg", false);

			// 再生する。
			int id_se1 = asd.Engine.Sound.Play(se1);
			int id_bgm1 = asd.Engine.Sound.Play(bgm1);

			// aceが進行可能かチェックする。
			while (asd.Engine.DoEvents())
			{
				// aceを更新する。
				asd.Engine.Update();

				// 音が再生終了してるか調べる。
				if(!asd.Engine.Sound.IsPlaying(id_bgm1))
				{
					break;
				}
			}

			// aceを終了する。
			asd.Engine.Terminate();
		}
	}
}
