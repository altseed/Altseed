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
			// AC-Engineを初期化する。
			ace.Engine.Initialize("Sound", 640, 480, new ace.EngineOption());

			// ファイルを読み込む
			ace.SoundSource se1 = ace.Engine.Sound.CreateSoundSource("Data/Sound/se1.wav", true);
			ace.SoundSource bgm1 = ace.Engine.Sound.CreateSoundSource("Data/Sound/bgm1.ogg", false);

			// 再生する。
			int id_se1 = ace.Engine.Sound.Play(se1);
			int id_bgm1 = ace.Engine.Sound.Play(bgm1);

			// AC-Engineのウインドウが閉じられていないか確認する。
			while (ace.Engine.DoEvents())
			{
				// AC-Engineを更新する。
				ace.Engine.Update();

				// 音が再生終了してるか調べる。
				if(!ace.Engine.Sound.IsPlaying(id_bgm1))
				{
					break;
				}
			}

			// AC-Engineの終了処理をする。
			ace.Engine.Terminate();
		}
	}
}
