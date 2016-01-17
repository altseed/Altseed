namespace sample_cs
{
	class Sound_BGM : ISample
    {
        public string Description
        {
            get { return ""; }
        }

        // Soundを用いてBGMを鳴らすサンプル。
        public void Run()
		{
			// Altseedを初期化する。
			asd.Engine.Initialize("Sound_BGM", 640, 480, new asd.EngineOption());

			// 音声ファイルを読み込む。BGMの場合、第２引数を false に設定することで、再生しながらファイルを解凍することが推奨されている。
			asd.SoundSource bgm1 = asd.Engine.Sound.CreateSoundSource("Data/Sound/bgm1.ogg", false);

			// 音声を再生する。
			int id_bgm1 = asd.Engine.Sound.Play(bgm1);

			// Altseedのウインドウが閉じられていないか確認する。
			while (asd.Engine.DoEvents())
			{
				// Altseedを更新する。
				asd.Engine.Update();

				// 音声が再生終了してるか調べる。
				if (!asd.Engine.Sound.GetIsPlaying(id_bgm1))
				{
					break;
				}
			}

			// Altseedの終了処理をする。
			asd.Engine.Terminate();
		}
	}
}
