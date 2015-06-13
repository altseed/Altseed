
/// <summary>
/// Soundを用いて音を鳴らすサンプルです。
/// </summary>
class Sound : ISample
{
	public void Run()
	{
		// Altseedを初期化する。
		asd.Engine.Initialize("Sound", 640, 480, new asd.EngineOption());

		// 音ファイルを読み込む
		asd.SoundSource se1 = asd.Engine.Sound.CreateSoundSource("Data/Sound/se1.wav", true);
		asd.SoundSource bgm1 = asd.Engine.Sound.CreateSoundSource("Data/Sound/bgm1.ogg", false);

		// 音を再生する。
		int id_se1 = asd.Engine.Sound.Play(se1);
		int id_bgm1 = asd.Engine.Sound.Play(bgm1);

		// Altseedのウインドウが閉じられていないか確認する。
		while (asd.Engine.DoEvents())
		{
			// Altseedを更新する。
			asd.Engine.Update();

			// 音が再生終了してるか調べる。
			if(!asd.Engine.Sound.IsPlaying(id_bgm1))
			{
				break;
			}
		}

		// Altseedの終了処理をする。
		asd.Engine.Terminate();
	}
}

