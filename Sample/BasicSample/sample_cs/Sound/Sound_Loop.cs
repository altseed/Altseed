using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace sample_cs
{
	class Sound_Loop : ISample
	{
		// BGMをループ再生するサンプル。
		public void Run()
		{
			// Altseedを初期化する。
			asd.Engine.Initialize("Sound", 640, 480, new asd.EngineOption());

			// 音声ファイルを読み込む
			asd.SoundSource bgm1 = asd.Engine.Sound.CreateSoundSource("Data/Sound/bgm1.ogg", false);

			// 音声のループを有効にする。
			bgm1.IsLoopingMode = true;

			// 音声のループ始端を1秒に、ループ終端を6秒に設定する。
			bgm1.LoopStartingPoint = 1.0f;
			bgm1.LoopEndPoint = 6.0f;

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
