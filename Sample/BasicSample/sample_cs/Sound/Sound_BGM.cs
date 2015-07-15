using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace sample_cs
{
	class Sound_BGM : ISample
	{
		// Soundを用いてBGMを鳴らすサンプル。
		public void Run()
		{
			// Altseedを初期化する。
			asd.Engine.Initialize("Sound_BGM", 640, 480, new asd.EngineOption());

			// 音声ファイルを読み込む
			asd.SoundSource bgm1 = asd.Engine.Sound.CreateSoundSource("Data/Sound/bgm1.ogg", false);

			// 音声を再生する。
			int id_bgm1 = asd.Engine.Sound.Play(bgm1);

			// Altseedのウインドウが閉じられていないか確認する。
			while (asd.Engine.DoEvents())
			{
				// Altseedを更新する。
				asd.Engine.Update();

				// 音声が再生終了してるか調べる。
				if (!asd.Engine.Sound.IsPlaying(id_bgm1))
				{
					break;
				}
			}

			// Altseedの終了処理をする。
			asd.Engine.Terminate();
		}
	}
}
